#include <errno.h>
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

// +===----- Directory -----===+ //

t_ErrorCode	fs_directory_create(t_Manager *manager, const t_Command *cmd)
{
	t_FileSystemCtx	*_ctx;
	t_CmdCreateDir	*_payload;
	t_Directory		*_parent_dir;
	t_Directory		*_dir;

	_ctx = manager->fs_ctx;
	_payload = cmd->payload;
	if (NULL == _payload->dirname || NULL == _payload->parent_dirname)
		return (ERR_INVALID_PAYLOAD);
	if (false == os_dir_create(_payload->dirname, _payload->mode))
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
	_parent_dir = recursive_find_sub_directory(_ctx->root, _payload->parent_dirname);
	if (NULL == _parent_dir)
		return (ERR_DIR_NOT_FOUND);
	_dir = directory_create();
	if (NULL == _dir)
		return (ERR_INTERNAL_MEMORY);
	_dir->absolute_path = ft_strdup(_payload->dirname);
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
	if (NULL == _payload->dirname || NULL == _payload->parent_dirname)
		return (ERR_INVALID_PAYLOAD);
	_parent_dir = recursive_find_sub_directory(_ctx->root, _payload->parent_dirname);
	if (NULL == _parent_dir)
		return (ERR_DIR_NOT_FOUND);
	_dir = directory_find_sub_directory(_parent_dir, _payload->dirname);
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
	if (false == directory_sub_directory_remove(_payload->parent_dirname, _dir))
		return (ERR_INTERNAL_MEMORY);
	directory_destroy(_dir);
	return (ERR_SUCCESS);
}

t_ErrorCode	fs_directory_move(t_Manager *manager, const t_Command *cmd);

// +===----- Files -----===+ //

t_ErrorCode	fs_file_create(t_Manager *manager, const t_Command *cmd);

t_ErrorCode	fs_file_delete(t_Manager *manager, const t_Command *cmd);

t_ErrorCode	fs_file_move(t_Manager *manager, const t_Command *cmd);

t_ErrorCode	fs_file_read(t_Manager *manager, const t_Command *cmd);

t_ErrorCode	fs_file_edit_data(t_Manager *manager, const t_Command *cmd);