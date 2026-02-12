#include "systems/filesystem/_internal.h"
#include "systems/filesystem/commands.h"
#include "systems/filesystem/_watcher.h"

// TODO: delete
static void	print_vfs_tree_node(const t_Directory *dir, const char *prefix, bool is_last)
{
	size_t			i;
	size_t			total;
	char			next_prefix[1024];
	const char		*name;

	if (NULL == dir)
		return ;
	name = dir->dirname;
	if (NULL == name || '\0' == name[0])
		name = ".";
	if (NULL == prefix)
		printf("%s\n", name);
	else
		printf("%s%s%s\n", prefix, is_last ? "`-- " : "|-- ", name);
	if (NULL == prefix)
		snprintf(next_prefix, sizeof(next_prefix), "");
	else
		snprintf(next_prefix, sizeof(next_prefix), "%s%s", prefix, is_last ? "    " : "|   ");
	total = dir->subdir_count + dir->files_count;
	i = 0;
	while (i < dir->subdir_count)
	{
		print_vfs_tree_node(dir->subdir[i], next_prefix, (i + 1 == total));
		i++;
	}
	i = 0;
	while (i < dir->files_count)
	{
		printf("%s%s%s\n", next_prefix, (dir->subdir_count + i + 1 == total)
			? "`-- " : "|-- ", dir->files[i]->filename);
		i++;
	}
}

// TODO: Delete
static void dbg_mask(uint32_t m) {
	if (m & IN_CREATE) printf("IN_CREATE ");
	if (m & IN_DELETE) printf("IN_DELETE ");
	if (m & IN_DELETE_SELF) printf("IN_DELETE_SELF ");
	if (m & IN_MOVE_SELF) printf("IN_MOVE_SELF ");
	if (m & IN_MOVED_FROM) printf("IN_MOVED_FROM ");
	if (m & IN_MOVED_TO) printf("IN_MOVED_TO ");
	if (m & IN_IGNORED) printf("IN_IGNORED ");
	if (m & IN_ISDIR) printf("IN_ISDIR ");
	if (m & IN_UNMOUNT) printf("IN_UNMOUNT ");
	printf("\n");
}

t_ErrorCode	get_VFS_root(t_Directory *root, const char *abs_path)
{
	DIR				*_dir;
	struct dirent	*_entry;
	struct stat		_st;
	char			*_entry_path;
	t_Directory		*_subdir;
	t_ErrorCode		_err;

	_dir = opendir(abs_path);
	if (NULL == _dir)
		return (get_dir_error());
	while ((_entry = readdir(_dir)) != NULL)
	{
		if (strcmp(_entry->d_name, ".") == 0
			|| strcmp(_entry->d_name, "..") == 0)
			continue ;
		_entry_path = join_path(abs_path, _entry->d_name);
		if (NULL == _entry_path)
			return (closedir(_dir), ERR_INTERNAL_MEMORY);
		if (stat(_entry_path, &_st) == -1)
			return (free(_entry_path), closedir(_dir), get_dir_error());
		if (S_ISDIR(_st.st_mode))
		{
			_subdir = directory_create(root, _entry->d_name);
			if (NULL == _subdir)
				return (free(_entry_path), closedir(_dir), ERR_INTERNAL_MEMORY);
			_err = get_VFS_root(_subdir, _entry_path);
			if (_err)
				return (free(_entry_path), closedir(_dir), _err);
		}
		else if (S_ISREG(_st.st_mode))
		{
			if (NULL == file_create(root, _entry->d_name))
				return (free(_entry_path), closedir(_dir), ERR_INTERNAL_MEMORY);
		}
		free(_entry_path);
	}
	closedir(_dir);
	return (ERR_SUCCESS);
}

/**
 * @brief Add a directory to the parent.
 * @param parent The parent directory.
 * @param dirname The dirname of the new directory.
*/
static bool	watch_add_directory(t_Directory *parent, char *dirname)
{
	t_Directory	*_dir;

	TEST_NULL(parent, false);
	TEST_NULL(dirname, false);
	_dir = directory_create(parent, dirname);
	TEST_NULL(_dir, false);
	return (true);
}

/**
 * @brief Delete a directory to the parent.
 * @param parent The parent directory.
 * @param dirname The dirname of the directory that was be deleted.
*/
static bool	watch_delete_directory(t_Directory *parent, char *dirname)
{
	t_Directory	*_dir;

	TEST_NULL(parent, false);
	TEST_NULL(dirname, false);
	_dir = directory_find_subdir(parent, dirname);
	TEST_NULL(_dir, false);
	TEST_ERROR_FN(directory_subdir_remove(parent, _dir), false);
	directory_destroy(_dir);
	return (true);
}

/**
 * @brief Add a file to the parent.
 * @param parent The parent directory.
 * @param filename The filename of the new file.
*/
static bool	watch_add_file(t_Directory *parent, char *filename)
{
	t_File	*_file;

	TEST_NULL(parent, false);
	TEST_NULL(filename, false);
	_file = file_create(parent, filename);
	TEST_NULL(_file, false);
	return (true);
}

/**
 * @brief Delete a file to the parent.
 * @param parent The parent directory.
 * @param filename The filename of the file that was be deleted.
*/
static bool	watch_delete_file(t_Directory *parent, char *filename)
{
	t_File	*_file;

	TEST_NULL(parent, false);
	TEST_NULL(filename, false);
	_file = directory_find_file(parent, filename);
	TEST_NULL(_file, false);
	TEST_ERROR_FN(directory_file_remove(parent, _file), false);
	file_destroy(_file);
	return (true);
}

static bool	handle_events(t_Directory *root, struct inotify_event *event)
{
	if (event->mask & IN_CREATE)
	{
		if (event->mask & IN_ISDIR)
		{
			TEST_ERROR_FN(watch_add_directory(root, event->name), false);
		}
		else
		{
			TEST_ERROR_FN(watch_add_file(root, event->name), false);
		}
	}
	else if (event->mask & IN_DELETE || event->mask & IN_MOVE)
	{
		if (event->mask & IN_ISDIR)
		{
			if (event->mask & IN_MOVE)
			{
			}
			TEST_ERROR_FN(watch_delete_directory(root, event->name), false);
		}
		else
		{
			TEST_ERROR_FN(watch_delete_file(root, event->name), false);
		}
	}
	return (true);
}

bool	apply_events(t_Directory *root, int inotify_fd)
{
	struct inotify_event	*_event;
	char					_buffer[4096];
	ssize_t					_len;
	char					*_ptr;

	_len = read(inotify_fd, _buffer, sizeof(_buffer));
	if (_len < 0)
		return (errno == EAGAIN);
	_ptr = _buffer;
	while (_ptr < _buffer + _len)
	{
		_event = (struct inotify_event *)_ptr;
		if (_event->mask & IN_Q_OVERFLOW)
			return (false);
		dbg_mask(_event->mask);
		if (_event->mask & IN_IGNORED || _event->mask & IN_MOVE_SELF
			|| _event->mask & IN_DELETE_SELF)
			return (false);
		handle_events(root, _event);
		_ptr += sizeof(struct inotify_event) + _event->len;
	}
	return (true);
}

int	main(int ac, char **av)
{
	int fd, wd;
	uint32_t mask;
	t_Directory	*_root;

	if (ac != 2)
		return (fprintf(stderr, "usage: %s /absolute_path_mon_dossier\n", av[0]), 1);

	fd = inotify_init1(0); // bloquant
	if (fd < 0)
		return (perror("inotify_init1"), 1);

	mask = IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_FROM | IN_MOVED_TO
		| IN_ATTRIB | IN_DELETE_SELF | IN_MOVE_SELF | IN_Q_OVERFLOW | IN_UNMOUNT;

	wd = inotify_add_watch(fd, av[1], mask);
	if (wd < 0)
		return (perror("inotify_add_watch"), close(fd), 1);

	_root = directory_create(NULL, "tmp");
	TEST_NULL(_root, 1);
	if (get_VFS_root(_root, av[1]))
		return (inotify_rm_watch(fd, wd), close(fd), directory_destroy(_root), 1);
	printf("watching %s (wd=%d)\n", av[1], wd);
	while (1)
	{
		printf("=== TREE ===\n");
		print_vfs_tree_node(_root, NULL, false);
		printf("=== END TREE ===\n");
		if (!apply_events(_root, fd))
		{
			printf("desync detected => full resync needed\n");
			printf("Resync...\n");
			directory_destroy(_root);
			_root = directory_create(NULL, "tmp");
			if (NULL == _root)
				break ;
			if (get_VFS_root(_root, av[1]))
				return (printf("Root deleted."), 1);
			printf("Resync will be successfully!\n");
		}
	}
	directory_destroy(_root);
	inotify_rm_watch(fd, wd);
	close(fd);
	return (0);
}
