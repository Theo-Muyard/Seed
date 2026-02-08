#include "seed.h"
#include "dependency.h"
#include "tools/memory.h"
#include "systems/filesystem/system.h"
#include "systems/filesystem/commands.h"
#include "systems/filesystem/_internal.h"
#include "systems/filesystem/_os.h"

// +===----- OS Errors -----===+ //

t_ErrorCode	get_file_error(void)
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
	return (ERR_OPERATION_FAILED);
}

t_ErrorCode	get_dir_error(void)
{
	switch (errno)
		{
			case EEXIST:
				return (ERR_DIR_EXIST);
			case EACCES:
				return (ERR_DIR_ACCESS);
			case ENOENT:
				return (ERR_DIR_NOT_FOUND);
			default:
				return (ERR_OPERATION_FAILED);
		}
	return (ERR_OPERATION_FAILED);
}

// +===----- Path -----===+ //

/**
 * 
*/
static char	*get_absolute_path(t_FileSystemCtx *ctx, const char *path)
{
	char	*abs_path;
	size_t	_size;
	size_t	_len;

	TEST_NULL(ctx, NULL);
	TEST_NULL(path, NULL);
	TEST_NULL(ctx->root_path, NULL);

	_len = strlen(path);
	_size = ctx->path_len + 1 + _len + 1;
	abs_path = malloc(_size * sizeof(char));
	TEST_NULL(abs_path, NULL);
	memcpy(abs_path, ctx->root_path, ctx->path_len);
	abs_path[ctx->path_len] = '/';
	memcpy(
		abs_path + ctx->path_len + 1,
		path,
		_len
	);
	abs_path[_size - 1] = '\0';
	return (abs_path);
}

/**
 * 
*/
static t_Directory	*get_parent_directory(t_Directory *root, const char *path)
{
	t_Directory	*_dir;
	char		*_cpy;
	char		*_slash;

	_cpy = ft_strdup(path);
	TEST_NULL(_cpy, NULL);
	_slash = strrchr(_cpy, '/');
	if (NULL == _slash)
		return (free(_cpy), root);
	*_slash = '\0';
	_dir = directory_resolve(root, _cpy);
	free(_cpy);
	return (_dir);
}

// +===----- Root -----===+ //

t_ErrorCode	cmd_root_open(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdOpenRoot	*_payload;
	t_Directory		*_root;
	char			*_dirname;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	TEST_NULL(_payload->path, ERR_INVALID_PAYLOAD);
	if (_ctx->root)
	{
		directory_destroy(_ctx->root);
		free(_ctx->root_path);
		_ctx->root = NULL;
		_ctx->root_path = NULL;
		_ctx->path_len = 0;
	}
	_dirname = strrchr(_payload->path, '/');
	TEST_NULL(_dirname, ERR_INVALID_PAYLOAD);
	_root = directory_create(NULL, _dirname + 1);
	TEST_NULL(_root, ERR_INTERNAL_MEMORY);
	_ctx->root = _root;
	_ctx->root_path = ft_strdup(_payload->path);
	if (NULL == _ctx->root_path)
		return (directory_destroy(_ctx->root), ERR_INTERNAL_MEMORY);
	_ctx->path_len = strlen(_ctx->root_path);
	return (ERR_SUCCESS);
}

t_ErrorCode	cmd_root_close(t_Manager *manager, const t_Command *cmd)
{
	(void)cmd;
	t_FileSystemCtx	*_ctx;

	_ctx = manager->fs_ctx;
	TEST_NULL(_ctx->root, ERR_FS_CONTEXT_NOT_INITIALIZED);
	directory_destroy(_ctx->root);
	free(_ctx->root_path);
	_ctx->root = NULL;
	_ctx->root_path = NULL;
	_ctx->path_len = 0;
	return (ERR_SUCCESS);
}

// +===----- Directory -----===+ //

t_ErrorCode	cmd_directory_create(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdCreateDir	*_payload;
	t_Directory		*_parent_dir;
	char			*_dirname;
	char			*_abs_path;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	TEST_NULL(_ctx->root, ERR_FS_CONTEXT_NOT_INITIALIZED);
	TEST_NULL(_payload->path, ERR_INVALID_PAYLOAD);
	_abs_path = get_absolute_path(_ctx, _payload->path);
	TEST_NULL(_abs_path, ERR_INTERNAL_MEMORY);
	TEST_OS_DIR_ERR(os_dir_create(_abs_path, 0755));
	_parent_dir = get_parent_directory(_ctx->root, _payload->path);
	if (NULL == _parent_dir)
		TEST_OS_DIR_ERR(os_dir_delete(_abs_path));
	_dirname = strrchr(_payload->path, '/');
	_dirname = _dirname ?  _dirname + 1 : _payload->path;
	TEST_NULL(directory_create(_parent_dir, _dirname), ERR_INTERNAL_MEMORY);
	free(_abs_path);
	return (ERR_SUCCESS);
}

t_ErrorCode	cmd_directory_delete(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdDeleteDir	*_payload;
	t_Directory		*_dir;
	char			*_abs_path;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	TEST_NULL(_ctx->root, ERR_FS_CONTEXT_NOT_INITIALIZED);
	TEST_NULL(_payload->path, ERR_INVALID_PAYLOAD);
	_abs_path = get_absolute_path(_ctx, _payload->path);
	TEST_NULL(_abs_path, ERR_INTERNAL_MEMORY);
	TEST_OS_DIR_ERR(os_dir_delete(_abs_path));
	_dir = directory_resolve(_ctx->root, _payload->path);
	TEST_NULL(_dir, ERR_SUCCESS);
	TEST_ERROR_FN(
		directory_subdir_remove(_dir->parent, _dir),
		ERR_OPERATION_FAILED
	);
	directory_destroy(_dir);
	free(_abs_path);
	return (ERR_SUCCESS);
}

t_ErrorCode	cmd_directory_move(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdMoveDir	*_payload;
	t_Directory		*_new_parent_dir;
	t_Directory		*_dir;
	char			*_old_abs_path;
	char			*_new_abs_path;
	char			*_slash;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	TEST_NULL(_ctx->root, ERR_FS_CONTEXT_NOT_INITIALIZED);
	TEST_NULL(_payload->old_path, ERR_INVALID_PAYLOAD);
	TEST_NULL(_payload->new_path, ERR_INVALID_PAYLOAD);
	_old_abs_path = get_absolute_path(_ctx, _payload->old_path);
	_new_abs_path = get_absolute_path(_ctx, _payload->new_path);
	TEST_NULL(_old_abs_path, ERR_OPERATION_FAILED);
	TEST_NULL(_new_abs_path, ERR_OPERATION_FAILED);
	TEST_OS_DIR_ERR(os_dir_move(_old_abs_path, _new_abs_path));
	_dir = directory_resolve(_ctx->root, _payload->old_path);
	_new_parent_dir = get_parent_directory(_ctx->root, _payload->new_path);
	TEST_NULL(_dir, ERR_DIR_NOT_FOUND);
	TEST_NULL(_new_parent_dir, ERR_DIR_NOT_FOUND);
	_slash = strrchr(_payload->new_path, '/');
	if (NULL == _slash)
	{
		if (false == directory_subdir_rename(_dir, _payload->new_path))
			return (free(_old_abs_path), free(_new_abs_path), ERR_INTERNAL_MEMORY);
	}
	else
	{
		if (false == directory_subdir_rename(_dir, _slash + 1))
			return (free(_old_abs_path), free(_new_abs_path), ERR_INTERNAL_MEMORY);
	}
	TEST_ERROR_FN(
		directory_subdir_move(_new_parent_dir, _dir->parent, _dir),
		ERR_OPERATION_FAILED
	);
	free(_old_abs_path);
	free(_new_abs_path);
	return (ERR_SUCCESS);
}

// +===----- Files -----===+ //

t_ErrorCode	cmd_file_create(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdCreateFile	*_payload;
	t_Directory		*_parent_dir;
	FILE			*_file;
	char			*_filename;
	char			*_abs_path;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	TEST_NULL(_ctx->root, ERR_FS_CONTEXT_NOT_INITIALIZED);
	TEST_NULL(_payload->path, ERR_INVALID_PAYLOAD);
	_abs_path = get_absolute_path(_ctx, _payload->path);
	TEST_NULL(_abs_path, ERR_INTERNAL_MEMORY);
	_file = os_file_create(_abs_path, "w");
	if (NULL == _file)
		return (free(_abs_path), ERR_OPERATION_FAILED);
	os_file_save(_file);
	_parent_dir = get_parent_directory(_ctx->root, _payload->path);
	if (NULL == _parent_dir)
		TEST_OS_FILE_ERR(os_file_delete(_abs_path));
	_filename = strrchr(_payload->path, '/');
	_filename = _filename ?  _filename + 1 : _payload->path;
	TEST_NULL(file_create(_parent_dir, _filename), ERR_INTERNAL_MEMORY);
	free(_abs_path);
	return (ERR_SUCCESS);
}

t_ErrorCode	cmd_file_delete(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdDeleteFile	*_payload;
	t_File			*_file;
	char			*_abs_path;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	TEST_NULL(_ctx->root, ERR_FS_CONTEXT_NOT_INITIALIZED);
	TEST_NULL(_payload->path, ERR_INVALID_PAYLOAD);
	_abs_path = get_absolute_path(_ctx, _payload->path);
	TEST_NULL(_abs_path, ERR_INTERNAL_MEMORY);
	TEST_OS_DIR_ERR(os_file_delete(_abs_path));
	_file = file_resolve(_ctx->root, _payload->path);
	TEST_NULL(_file, ERR_SUCCESS);
	TEST_ERROR_FN(
		directory_file_remove(_file->parent, _file),
		ERR_OPERATION_FAILED
	);
	file_destroy(_file);
	free(_abs_path);
	return (ERR_SUCCESS);
}

t_ErrorCode	cmd_file_move(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdMoveFile	*_payload;
	t_Directory		*_new_parent_dir;
	t_File			*_file;
	char			*_old_abs_path;
	char			*_new_abs_path;
	char			*_slash;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	TEST_NULL(_ctx->root, ERR_FS_CONTEXT_NOT_INITIALIZED);
	TEST_NULL(_payload->old_path, ERR_INVALID_PAYLOAD);
	TEST_NULL(_payload->new_path, ERR_INVALID_PAYLOAD);
	_old_abs_path = get_absolute_path(_ctx, _payload->old_path);
	_new_abs_path = get_absolute_path(_ctx, _payload->new_path);
	TEST_NULL(_old_abs_path, ERR_OPERATION_FAILED);
	TEST_NULL(_new_abs_path, ERR_OPERATION_FAILED);
	TEST_OS_DIR_ERR(os_file_move(_old_abs_path, _new_abs_path));
	_file = file_resolve(_ctx->root, _payload->old_path);
	_new_parent_dir = get_parent_directory(_ctx->root, _payload->new_path);
	TEST_NULL(_file, ERR_DIR_NOT_FOUND);
	TEST_NULL(_new_parent_dir, ERR_DIR_NOT_FOUND);
	_slash = strrchr(_payload->new_path, '/');
	if (NULL == _slash)
	{
		if (false == directory_file_rename(_file, _payload->new_path))
			return (free(_old_abs_path), free(_new_abs_path), ERR_INTERNAL_MEMORY);
	}
	else
	{
		if (false == directory_file_rename(_file, _slash + 1))
			return (free(_old_abs_path), free(_new_abs_path), ERR_INTERNAL_MEMORY);
	}
	TEST_ERROR_FN(
		directory_file_move(_new_parent_dir, _file->parent, _file),
		ERR_OPERATION_FAILED
	);
	free(_old_abs_path);
	free(_new_abs_path);
	return (ERR_SUCCESS);
}

t_ErrorCode	cmd_file_read(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdReadFile	*_payload;
	FILE			*_file;
	char			*_data;
	char			*_abs_path;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	TEST_NULL(_ctx->root, ERR_FS_CONTEXT_NOT_INITIALIZED);
	TEST_NULL(_payload->path, ERR_INVALID_PAYLOAD);
	_abs_path = get_absolute_path(_ctx, _payload->path);
	TEST_NULL(_abs_path, ERR_OPERATION_FAILED);
	_file = os_file_open(_abs_path, "r");
	free(_abs_path);
	if (NULL == _file)
		return (ERR_OPERATION_FAILED);
	_data = os_file_get_data(_file);
	if (NULL == _data)
		return (os_file_save(_file), ERR_OPERATION_FAILED);
	_payload->out_data = _data;
	_payload->out_len = strlen(_data);
	os_file_save(_file);
	return (ERR_SUCCESS);
}

t_ErrorCode	cmd_file_edit_data(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx		*_ctx;
	t_CmdEditDataFile	*_payload;
	FILE				*_file;
	char				*_abs_path;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	TEST_NULL(_ctx->root, ERR_FS_CONTEXT_NOT_INITIALIZED);
	TEST_NULL(_payload->path, ERR_INVALID_PAYLOAD);
	TEST_NULL(_payload->data, ERR_INVALID_PAYLOAD);
	_abs_path = get_absolute_path(_ctx, _payload->path);
	TEST_NULL(_abs_path, ERR_OPERATION_FAILED);
	_file = os_file_open(_abs_path, "w");
	free(_abs_path);
	if (NULL == _file)
		return (ERR_OPERATION_FAILED);
	if (false == os_file_edit_data(_file, _payload->data))
		return (os_file_save(_file), ERR_OPERATION_FAILED);
	os_file_save(_file);
	return (ERR_SUCCESS);
}
