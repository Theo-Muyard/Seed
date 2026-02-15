#include "systems/filesystem/vfs/_internal.h"
#include "systems/filesystem/watcher/_internal.h"
#include "tools/memory.h"

#define ENTRY_ALLOC	32

#define WATCH_MASK	(IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_FROM \
	| IN_MOVED_TO | IN_ATTRIB | IN_DELETE_SELF | IN_MOVE_SELF \
	| IN_Q_OVERFLOW | IN_UNMOUNT | IN_IGNORED)

// +===----- Static functions -----===+ //

/**
 * @brief Check if the given path is a directory.
 * @param path The absolute path of the entry.
 * @return TRUE for directory, FALSE else.
*/
static bool	is_dir(const char *path)
{
	struct stat	_st;

	TEST_NULL(path, false);
	if (0 != lstat(path, &_st))
		return (false);
	return (S_ISDIR(_st.st_mode));
}

/**
 * @brief Get the path of the entry.
 * @param ctx The watcher context.
 * @param wd The wd of the entry.
*/
static char	*watch_get_path(t_WatchCtx *ctx, int wd)
{
	size_t			_i;
	t_WatchEntry	*_entry;

	_i = 0;
	while (_i < ctx->entry_count)
	{
		_entry = ctx->entries[_i];
		if (_entry->wd == wd)
			return (_entry->path);
		_i++;
	}
	return (NULL);
}

/**
 * @brief Realloc the pending capacity if needed.
 * @param ctx The watcher context.
 * @param new_count The new count of pending entries needed.
 * @return TRUE for success or FALSE if an error occured.
*/
static bool	pending_reserve(t_WatchCtx *ctx, size_t new_count)
{
	t_MovePending	**_tmp;
	size_t			_new_cap;
	
	if (new_count <= ctx->pending_capacity)
		return (true);
	if (0 == ctx->pending_capacity)
	{
		ctx->pending = malloc(ENTRY_ALLOC * sizeof(t_MovePending *));
		TEST_NULL(ctx->pending, false);
		ctx->pending_capacity = ENTRY_ALLOC;
	}
	else
	{
		_new_cap = ctx->pending_capacity;
		while (_new_cap < new_count)
			_new_cap *= 2;
		_tmp = realloc(ctx->pending, _new_cap * sizeof(t_MovePending *));
		TEST_NULL(_tmp, false);
		ctx->pending = _tmp;
		ctx->pending_capacity = _new_cap;
	}
	return (true);
}

/**
 * @brief Realloc the entries capacity if needed.
 * @param ctx The watcher context.
 * @param new_count The new count of watch entries needed.
 * @return TRUE for success or FALSE if an error occured.
*/
static bool	entries_reserve(t_WatchCtx *ctx, size_t new_count)
{
	t_WatchEntry	**_tmp;
	size_t			_new_cap;
	
	if (new_count <= ctx->entry_capacity)
		return (true);
	if (0 == ctx->entry_capacity)
	{
		ctx->entries = malloc(ENTRY_ALLOC * sizeof(t_WatchEntry *));
		TEST_NULL(ctx->entries, false);
		ctx->entry_capacity = ENTRY_ALLOC;
	}
	else
	{
		_new_cap = ctx->entry_capacity;
		while (_new_cap < new_count)
			_new_cap *= 2;
		_tmp = realloc(ctx->entries, _new_cap * sizeof(t_WatchEntry *));
		TEST_NULL(_tmp, false);
		ctx->entries = _tmp;
		ctx->entry_capacity = _new_cap;
	}
	return (true);
}

/**
 * @brief Add a new watch entry.
 * @param ctx The watcher context.
 * @param path The absolute path of the entry.
 * @return TRUE for success or FALSE if an error occured.
*/
static bool	watch_add_one(t_WatchCtx *ctx, const char *path)
{
	int				_wd;
	t_WatchEntry	*_entry;

	_wd = inotify_add_watch(ctx->fd, path, WATCH_MASK);
	if (_wd < 0)
		return (false);
	if (false == entries_reserve(ctx, ctx->entry_count + 1))
		return (inotify_rm_watch(ctx->fd, _wd), false);
	_entry = malloc(sizeof(t_WatchEntry));
	TEST_NULL(_entry, false);
	_entry->wd = _wd;
	_entry->path = ft_strdup(path);
	if (NULL == _entry->path)
		return (inotify_rm_watch(ctx->fd, _wd), false);
	ctx->entries[ctx->entry_count] = _entry;
	ctx->entry_count++;
	return (true);
}

/**
 * @brief Add recursive watch entries.
 * @param ctx The watcher context.
 * @param path The absolute path of the entry.
 * @return TRUE for success or FALSE if an error occured.
*/
static bool	watch_add_recursive(t_WatchCtx *ctx, const char *path)
{
	DIR				*_dir;
	struct dirent	*_entry;
	char			*_child_path;

	if (!is_dir(path))
		return (true);
	if (false == watch_add_one(ctx, path))
		return (false);
	_dir = opendir(path);
	TEST_NULL(_dir, false);
	while ((_entry = readdir(_dir)) != NULL)
	{
		if (strcmp(_entry->d_name, ".") == 0 || strcmp(_entry->d_name, "..") == 0)
			continue ;
		_child_path = join_path(path, _entry->d_name);
		TEST_NULL(_child_path, false);
		if (is_dir(_child_path) && false == watch_add_recursive(ctx, _child_path))
			return (free(_child_path), closedir(_dir), false);
		free(_child_path);
	}
	closedir(_dir);
	return (true);
}

/**
 * @brief Remove a watch entry by this wd.
 * @param ctx The watch context.
 * @param wd The wd of the entry.
*/
static void	watch_remove_one(t_WatchCtx *ctx, int wd)
{
	size_t			_i;
	t_WatchEntry	*_entry;

	_i = 0;
	while (_i < ctx->entry_count)
	{
		_entry = ctx->entries[_i];
		if (_entry->wd == wd)
		{
			free(_entry->path);
			free(_entry);
			memmove(
				ctx->entries + _i,
				ctx->entries + _i + 1,
				(ctx->entry_count - _i - 1) * sizeof(t_WatchEntry *)
			);
		}
		_i++;
	}
}

/**
 * @brief Remove recursive watch entries.
 * @param ctx The watcher context.
 * @param path The absolute path of the entry.
 * @return TRUE for success or FALSE if an error occured.
*/
static bool	watch_remove_recursive(t_WatchCtx *ctx, const char *path)
{
	DIR				*_dir;
	struct dirent	*_entry;
	char			*_child_path;

	if (!is_dir(path))
		return (true);
	watch_remove_one(ctx, path);
	_dir = opendir(path);
	TEST_NULL(_dir, false);
	while ((_entry = readdir(_dir) != NULL))
	{
		if (strcmp(_entry->d_name, ".") == 0 || strcmp(_entry->d_name, "..") == 0)
			continue ;
		_child_path = join_path(path, _entry->d_name);
		TEST_NULL(_child_path, false);
		if (is_dir(_child_path))
			watch_remove_recursive(ctx, _child_path);
		free(_child_path);
	}
	closedir(_dir);
	return (true);
}

static t_FsEvents	*handle_event
(
	t_WatchCtx	*ctx,
	struct inotify_event *event,
	const char *path
)
{
	t_FsEvents	ev;

	ev.isdir = false;
	if ((event->mask & IN_CREATE) && (event->mask & IN_ISDIR))
	{
		printf("Create dir: %s\n", path);
		if (false == watch_add_recursive(ctx, path))
			return (NULL);
		ev.isdir = true;
		ev.type = FS_EVENT_CREATE;
	}
	else if (event->mask & IN_CREATE)
	{
		printf("Create file: %s\n", path);
		ev.type = FS_EVENT_CREATE;
	}
	if ((event->mask & IN_DELETE) && (event->mask & IN_ISDIR))
	{
		printf("Delete dir: %s\n", path);
		if (false == watch_remove_recursive(ctx, path))
			return (NULL);
		ev.isdir = true;
		ev.type = FS_EVENT_DELETE;
	}
	else if (event->mask & IN_DELETE)
	{
		printf("Delete file: %s\n", path);
		ev.type = FS_EVENT_DELETE;
	}
	if ((event->mask & IN_MOVED_FROM) &&  (event->mask & IN_ISDIR))
	{
		printf("Move from dir: %s\n", path);
		ev.isdir = true;
		ev.type = FS_EVENT_MOVE;
	}
	else if (event->mask & IN_MOVED_FROM)
	{
		printf("Move from file: %s\n", path);
		ev.type = FS_EVENT_MOVE;
	}
	if ((event->mask & IN_MOVED_TO) & (event->mask & IN_ISDIR))
	{
		printf("Move to dir: %s\n", path);
		ev.isdir = true;
		ev.type = FS_EVENT_MOVE;
	}
	else if (event->mask & IN_MOVED_TO)
	{
		printf("Move to file: %s\n", path);
		ev.type = FS_EVENT_MOVE;
	}
	return (&ev);
}

// +===----- Functions -----===+ //

t_WatchCtx	*watcher_init(int fd, const char *path)
{
	t_WatchCtx	*watcher;

	watcher = malloc(sizeof(t_WatchCtx));
	TEST_NULL(watcher, NULL);
	watcher->fd = fd;
	watcher->path = ft_strdup(path);
	if (NULL == watcher->path)
		return (free(watcher), NULL);
	watcher->entries = NULL;
	watcher->entry_count = 0;
	watcher->entry_capacity = 0;

	watcher->pending = NULL;
	watcher->pending_count = 0;
	watcher->pending_capacity = 0;
	return (watcher);
}

void		watcher_destroy(t_WatchCtx *ctx)
{
	size_t	_i;

	if (NULL == ctx)
		return ;

	_i = 0;
	while (_i < ctx->entry_count)
	{
		free(ctx->entries[_i]->path);
		free(ctx->entries[_i]);
		_i++;
	}
	free(ctx->entries);
	_i = 0;
	while (_i < ctx->pending_count)
	{
		free(ctx->pending[_i]->from_path);
		free(ctx->pending[_i]);
		_i++;
	}
	free(ctx->pending);
	free(ctx->path);
	free(ctx);
}

bool	*watcher_analyze(t_WatchCtx *ctx)
{
	t_FsEvents				*ev;
	struct inotify_event	*_event;
	char					_buffer[4096];
	char					*_ptr;
	size_t					_len;
	char					*_parent_path;
	char					*_entry_path;

	_len = read(ctx->fd, _buffer, sizeof(_buffer));
	if (_len < 0)
		return (errno == EAGAIN || errno == EINTR);
	_ptr = _buffer;
	while (_ptr < _buffer + _len)
	{
		_event = (struct inotify_event *)_ptr;
		_parent_path = watch_get_path(ctx, _event->wd);
		if (_event->mask & IN_Q_OVERFLOW || NULL == _parent_path)
			return (NULL);
		_entry_path = join_path(_parent_path, _event->name);
		TEST_NULL(_entry_path, NULL);
		ev = handle_event(ctx, _event, _entry_path);
		free(_entry_path);
		_ptr += sizeof(struct inotify_event) + _event->len;
	}
	return (NULL);
}
