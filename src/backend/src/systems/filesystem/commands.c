#include <errno.h>
#include <string.h>
#include "seed.h"
#include "tools/memory.h"
#include "systems/filesystem/system.h"
#include "systems/filesystem/commands.h"
#include "systems/filesystem/_internal.h"
#include "systems/filesystem/_os.h"

/**
 * 
*/
static t_Directory	*recursive_find_sub_directory(t_Directory *root, char *dirname)
{
	size_t		_i;
	t_Directory	*_dir;

	if (NULL == root || NULL == dirname)
		return (NULL);
	_i = 0;
	_dir = directory_find_sub_directory(root->sub_directory, dirname);
	if (NULL != _dir)
		return (_dir);
	while (_i < root->sub_dir_count)
	{
		_dir = recursive_find_sub_directory(root->sub_directory[_i], dirname);
		if (NULL != _dir)
			return (_dir);
	}
	return (NULL);
}

/**
 * 
*/
static t_File	*recursive_find_file(t_Directory *root, char *filename)
{
	size_t	_i;
	t_File	*_file;

	if (NULL == root || NULL == filename)
		return (NULL);
	_i = 0;
	_file = directory_find_file(root->files, filename);
	if (NULL != _file)
		return (_file);
	while (_i < root->sub_dir_count)
	{
		_file = recursive_find_file(root->sub_directory[_i], filename);
		if (NULL != _file)
			return (_file);
	}
	return (NULL);
}

static char	*get_parent_path(const char *path)
{
	char	*_cpy;
	char	*_slash;

	if (NULL == path)
		return (NULL);
	_cpy = ft_strdup(path);
	if (NULL == _cpy)
		return (NULL);
	_slash = strrchr(_cpy, '/');
	if (NULL == _slash)
		return (free(_cpy), NULL);
	if (_cpy == _slash)
		_slash[1] = '\0';
	else
		*_slash = '\0';
	return (_cpy);
}

// +===----- Directory -----===+ //

t_ErrorCode	fs_directory_create(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdCreateDir	*_payload;
	t_Directory		*_parent_dir;
	t_Directory		*_dir;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	if (NULL == _payload->path)
		return (ERR_INVALID_PAYLOAD);
	if (false == os_dir_create(_payload->path, _payload->mode))
	{
		switch (errno)
		{
			case EEXIST:
				return (ERR_DIR_EXIST);
			case EACCES:
				return (ERR_DIR_ACCESS);
			default:
				return (ERR_OPERATION_FAILED);
		}
	}
	_parent_dir = recursive_find_sub_directory(_ctx->root, get_parent_path(_payload->path));
	if (NULL == _parent_dir)
		return (ERR_DIR_NOT_FOUND);
	_dir = directory_create();
	if (NULL == _dir)
		return (ERR_INTERNAL_MEMORY);
	_dir->absolute_path = ft_strdup(_payload->path);
	if (NULL == _dir->absolute_path)
		return (ERR_INTERNAL_MEMORY);
	if (false == directory_sub_directory_add(_parent_dir, _dir))
		return (directory_destroy(_dir), ERR_INTERNAL_MEMORY);
	return (ERR_SUCCESS);
}

t_ErrorCode	fs_directory_delete(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdDeleteDir	*_payload;
	t_Directory		*_parent_dir;
	t_Directory		*_dir;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	if (NULL == _payload->path)
		return (ERR_INVALID_PAYLOAD);
	_parent_dir = recursive_find_sub_directory(_ctx->root, get_parent_path(_payload->path));
	if (NULL == _parent_dir)
		return (ERR_DIR_NOT_FOUND);
	_dir = directory_find_sub_directory(_parent_dir, _payload->path);
	if (NULL == _dir)
		return (ERR_DIR_NOT_FOUND);
	if (false == os_dir_delete(_dir->absolute_path))
	{
		switch (errno)
		{
			case EACCES:
				return (ERR_DIR_ACCESS);
			case ENOENT:
				return (ERR_DIR_NOT_FOUND);
			default:
				return (ERR_OPERATION_FAILED);
		}
	}
	if (false == directory_sub_directory_remove(_parent_dir, _dir))
		return (ERR_INTERNAL_MEMORY);
	directory_destroy(_dir);
	return (ERR_SUCCESS);
}

t_ErrorCode	fs_directory_move(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdMoveDir	*_payload;
	t_Directory		*_new_parent_dir;
	t_Directory		*_old_parent_dir;
	t_Directory		*_dir;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	if (NULL == _payload->old_path || NULL == _payload->new_path)
		return (ERR_INVALID_PAYLOAD);
	if (false == os_dir_edit_path(_payload->old_path, _payload->new_path))
	{
		switch (errno)
		{
			case EEXIST:
				return (ERR_FILE_EXIST);
			case EACCES:
				return (ERR_FILE_ACCESS);
			case ENOENT:
				return (ERR_FILE_NOT_FOUND);
			default:
				return (ERR_OPERATION_FAILED);
		}
	}
	_old_parent_dir = recursive_find_sub_directory(_ctx->root, get_parent_path(_payload->old_path));
	_new_parent_dir = recursive_find_sub_directory(_ctx->root, get_parent_path(_payload->new_path));
	_dir = recursive_find_sub_directory(_ctx->root, _payload->old_path);
	if (NULL == _old_parent_dir || NULL == _new_parent_dir || NULL == _dir)
		return (ERR_DIR_NOT_FOUND);
	if (false == directory_sub_directory_move(_old_parent_dir, _new_parent_dir, _dir))
		return (ERR_INTERNAL_MEMORY);
	return (ERR_SUCCESS);
}

// +===----- Files -----===+ //

t_ErrorCode	fs_file_create(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdCreateFile	*_payload;
	t_Directory		*_parent_dir;
	t_File			*_file;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	if (NULL == _payload->path)
		return (ERR_INVALID_PAYLOAD);
	if (false == os_file_create(_payload->path, _payload->mode))
	{
		switch (errno)
		{
			case EEXIST:
				return (ERR_FILE_EXIST);
			case EACCES:
				return (ERR_FILE_ACCESS);
			default:
				return (ERR_OPERATION_FAILED);
		}
	}
	_parent_dir = recursive_find_sub_directory(_ctx->root, get_parent_path(_payload->path));
	if (NULL == _parent_dir)
		return (ERR_DIR_NOT_FOUND);
	_file = file_create();
	if (NULL == _file)
		return (ERR_INTERNAL_MEMORY);
	_file->absolute_path = ft_strdup(_payload->path);
	if (NULL == _file->absolute_path)
		return (ERR_INTERNAL_MEMORY);
	if (false == directory_file_add(_parent_dir, _file))
		return (file_destroy(_file), ERR_INTERNAL_MEMORY);
	return (ERR_SUCCESS);
}

t_ErrorCode	fs_file_delete(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdCreateFile	*_payload;
	t_Directory		*_parent_dir;
	t_File			*_file;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	if (NULL == _payload->path)
		return (ERR_INVALID_PAYLOAD);
	if (false == os_file_delete(_payload->path))
	{
		switch (errno)
		{
			case EACCES:
				return (ERR_FILE_ACCESS);
			case ENOENT:
				return (ERR_FILE_NOT_FOUND);
			default:
				return (ERR_OPERATION_FAILED);
		}
	}
	_parent_dir = recursive_find_sub_directory(_ctx->root, get_parent_path(_payload->path));
	_file = recursive_find_file(_ctx->root, _payload->path);
	if (NULL == _parent_dir)
		return (ERR_DIR_NOT_FOUND);
	if (NULL == _file)
		return (ERR_FILE_NOT_FOUND);
	if (false == directory_file_remove(_parent_dir, _file))
		return (ERR_INTERNAL_MEMORY);
	file_destroy(_file);
	return (ERR_SUCCESS);
}

t_ErrorCode	fs_file_move(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdMoveDir	*_payload;
	t_Directory		*_new_parent_dir;
	t_Directory		*_old_parent_dir;
	t_File			*_file;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	if (NULL == _payload->old_path || NULL == _payload->new_path)
		return (ERR_INVALID_PAYLOAD);
	if (false == os_file_edit_path(_payload->old_path, _payload->new_path))
	{
		switch (errno)
		{
			case EEXIST:
				return (ERR_FILE_EXIST);
			case EACCES:
				return (ERR_FILE_ACCESS);
			case ENOENT:
				return (ERR_FILE_NOT_FOUND);
			default:
				return (ERR_OPERATION_FAILED);
		}
	}
	_old_parent_dir = recursive_find_sub_directory(_ctx->root, get_parent_path(_payload->old_path));
	_new_parent_dir = recursive_find_sub_directory(_ctx->root, get_parent_path(_payload->new_path));
	_file = recursive_find_file(_ctx->root, _payload->old_path);
	if (NULL == _old_parent_dir || NULL == _new_parent_dir)
		return (ERR_DIR_NOT_FOUND);
	if (NULL == _file)
		return (ERR_FILE_NOT_FOUND);
	if (false == directory_file_move(_old_parent_dir, _new_parent_dir, _file))
		return (ERR_INTERNAL_MEMORY);
	return (ERR_SUCCESS);
}

t_ErrorCode	fs_file_read(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdReadFile	*_payload;
	FILE			*_file;
	char			*data;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	if (NULL == _payload->path)
		return (ERR_INVALID_PAYLOAD);
	_file = os_file_open(_payload->path, "r");
	data = os_file_get_data(_file);
	if (NULL == data)
	{
		switch (errno)
		{
			case EACCES:
				return (ERR_FILE_ACCESS);
			case ENOENT:
				return (ERR_FILE_NOT_FOUND);
			default:
				return (ERR_OPERATION_FAILED);
		}
	}
	_payload->out_data = data;
	_payload->out_len = strlen(data);
	return (ERR_SUCCESS);
}

t_ErrorCode	fs_file_edit_data(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx		*_ctx;
	t_CmdEditDataFile	*_payload;
	FILE				*_file;
	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	if (NULL == _payload->path || NULL == _payload->data)
		return (ERR_INVALID_PAYLOAD);
	_file = os_file_open(_payload->path, "w");
	if (NULL == os_file_edit_data(_file, _payload->data))
	{
		switch (errno)
		{
			case EACCES:
				return (ERR_FILE_ACCESS);
			case ENOENT:
				return (ERR_FILE_NOT_FOUND);
			default:
				return (ERR_OPERATION_FAILED);
		}
	}
	return (ERR_SUCCESS);
}