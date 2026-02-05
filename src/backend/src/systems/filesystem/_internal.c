#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "systems/filesystem/_internal.h"

#define FILES_ALLOC	32
#define	DIR_ALLOC	32

// +===----- Update -----===+ //

static bool	update_file_path(t_Directory *dst, t_File *file)
{
	char	*_tmp;
	char	*_filename;
	size_t	_new_len;

	if (NULL == dst || NULL == file)
		return (false);
	if (NULL == dst->absolute_path || NULL == file->absolute_path)
		return (false);
	_filename = strrchr(file->absolute_path, '/');
	if (NULL == _filename)
		return (false);
	_filename++;
	_new_len = strlen(dst->absolute_path) + 1 + strlen(_filename);
	_tmp = realloc(file->absolute_path, (_new_len + 1) * sizeof(char));
	if (NULL == _tmp)
		return (false);
	file->absolute_path = _tmp;
	snprintf(file->absolute_path, _new_len + 1, "%s/%s", dst->absolute_path, _filename);
	return (true);
}

static bool	update_sub_directory_path(t_Directory *dst, t_Directory *sub_dir)
{
	char	*_tmp;
	char	*_dirname;
	size_t	_new_len;
	size_t	_i;

	if (NULL == dst || NULL == sub_dir)
		return (false);
	if (NULL == dst->absolute_path || NULL == sub_dir->absolute_path)
		return (false);
	_dirname = strrchr(sub_dir->absolute_path, '/');
	if (NULL == _dirname)
		return (false);
	_dirname++;
	_new_len = strlen(dst->absolute_path) + 1 + strlen(_dirname);
	_tmp = realloc(sub_dir->absolute_path, (_new_len + 1) * sizeof(char));
	if (NULL == _tmp)
		return (false);
	sub_dir->absolute_path = _tmp;
	snprintf(sub_dir->absolute_path, _new_len + 1, "%s/%s", dst->absolute_path, _dirname);
	_i = 0;
	while (_i < sub_dir->files_count)
	{
		if (NULL != sub_dir->files[_i])
		{
			if (false == update_file_path(sub_dir, sub_dir->files[_i]))
				return (false);
		}
		_i++;
	}
	_i = 0;
	while (_i < sub_dir->sub_dir_count)
	{
		if (NULL != sub_dir->sub_directory[_i])
		{
			if (false == update_sub_directory_path(sub_dir, sub_dir->sub_directory[_i]))
				return (false);
		}
		_i++;
	}
	return (true);
}

// +===----- Directory -----===+ //

t_Directory	*directory_create(void)
{
	t_Directory	*dir;

	dir = malloc(sizeof(t_Directory));
	if (NULL == dir)
		return (NULL);
	dir->absolute_path = NULL;
	dir->files = NULL;
	dir->files_count = 0;
	dir->files_capacity = 0;
	dir->sub_directory = NULL;
	dir->sub_dir_count = 0;
	dir->sub_dir_capacity = 0;
	return (dir);
}

void		directory_destroy(t_Directory *dir)
{
	size_t _i;

	if (NULL == dir)
		return ;
	_i = 0;
	while (_i < dir->files_count)
	{
		file_destroy(dir->files[_i]);
		_i++;
	}
	free(dir->files);
	_i = 0;
	while (_i < dir->sub_dir_count)
	{
		directory_destroy(dir->sub_directory[_i]);
		_i++;
	}
	free(dir->absolute_path);
	free(dir->sub_directory);
	free(dir);
}

// +===----- Files -----===+ //

t_File		*file_create(void)
{
	t_File	*file;

	file = malloc(sizeof(t_File));
	if (NULL == file)
		return (NULL);
	file->buffer_id = -1;
	file->absolute_path = NULL;
	return (file);
}

void		file_destroy(t_File *file)
{
	if (NULL == file)
		return ;
	free(file->absolute_path);
	free(file);
}

bool		directory_file_add(t_Directory *dir, t_File *file)
{
	t_File	*_tmp;

	if (NULL == dir || NULL == file)
		return (false);
	if (NULL == dir->files)
	{
		dir->files = malloc(FILES_ALLOC * sizeof(t_File *));
		if (NULL == dir->files)
			return (false);
		bzero(dir->files, FILES_ALLOC * sizeof(t_File *));
		dir->files_capacity = FILES_ALLOC;
	}
	if (dir->files_count >= dir->files_capacity)
	{
		_tmp = realloc(
			dir->files,
			(FILES_ALLOC + dir->files_capacity) * sizeof(t_File *)
		);
		if (NULL == _tmp)
			return (false);
		dir->files = _tmp;
		dir->files_capacity += FILES_ALLOC;
		bzero(
			dir->files + dir->files_count,
			FILES_ALLOC * sizeof(t_File *)
		);
	}
	dir->files[dir->files_count] = file;
	dir->files_count++;
	return (true);
}

bool		directory_file_remove(t_Directory *dir, t_File *file)
{
	size_t	_i;

	if (NULL == dir || NULL == file)
		return (false);
	_i = 0;
	while (_i < dir->files_count && dir->files[_i] != file)
		_i++;
	if (dir->files[_i] != file)
		return (false);
	dir->files[_i] = NULL;
	memmove(
		dir->files + _i,
		dir->files + _i + 1,
		(dir->files_count - _i - 1) * sizeof(t_File *)
	);
	dir->files_count--;
	return (true); 
}

t_File		*directory_find_file(t_Directory *dir, char *path)
{
	size_t	_i;

	if (NULL == dir || NULL == path)
		return (NULL);
	_i = 0;
	while (_i < dir->files_count)
	{
		if (NULL != dir->files[_i])
		{
			if (NULL == dir->files[_i]->absolute_path)
			{
				_i++;
				continue;
			}
			if (strcmp(dir->files[_i]->absolute_path, path) == 0)
				return (dir->files[_i]);
		}
		_i++;
	}
	return (NULL);
}

bool		directory_file_move(t_Directory *src, t_Directory *dst, t_File *file)
{
	if (NULL == src || NULL == dst || NULL == file)
		return (false);
	if (false == directory_file_remove(src, file))
		return (false);
	if (false == directory_file_add(dst, file))
		return (false);
	if (false == update_file_path(dst, file))
		return (false);
	return (true);
}

bool		directory_contains_file(t_Directory *dir, t_File *file)
{
	size_t	_i;

	if (NULL == dir || NULL == file)
		return (false);
	_i = 0;
	while (_i < dir->files_count)
	{
		if (file == dir->files[_i])
			return (true);
		_i++;
	}
	return (false);
}

// +===----- Sub directory -----===+ //

bool		directory_sub_directory_add(t_Directory *dir, t_Directory *sub_dir)
{
	t_Directory	*_tmp;

	if (NULL == dir || NULL == sub_dir)
		return (false);
	if (NULL == dir->sub_directory)
	{
		dir->sub_directory = malloc(DIR_ALLOC * sizeof(t_Directory *));
		if (NULL == dir->sub_directory)
			return (false);
		bzero(dir->sub_directory, DIR_ALLOC * sizeof(t_Directory *));
		dir->sub_dir_capacity = DIR_ALLOC;
	}
	if (dir->sub_dir_count >= dir->sub_dir_capacity)
	{
		_tmp = realloc(
			dir->sub_directory,
			DIR_ALLOC * sizeof(t_Directory *)
		);
		if (NULL == _tmp)
			return (false);
		dir->sub_directory = _tmp;
		dir->sub_dir_capacity += DIR_ALLOC;
		bzero(
			dir->sub_directory + dir->sub_dir_count,
			DIR_ALLOC * sizeof(t_Directory *)
		);
	}
	dir->sub_directory[dir->sub_dir_count] = sub_dir;
	dir->sub_dir_count++;
	return (true);
}

bool		directory_sub_directory_remove(t_Directory *dir, t_Directory *sub_dir)
{
	size_t	_i;

	if (NULL == dir || NULL == sub_dir)
		return (false);
	_i = 0;
	while (_i < dir->sub_dir_count && dir->sub_directory[_i] != sub_dir)
		_i++;
	if (dir->sub_directory[_i] != sub_dir)
		return (false);
	dir->sub_directory[_i] = NULL;
	memmove(
		dir->sub_directory + _i,
		dir->sub_directory + _i + 1,
		(dir->sub_dir_count - _i - 1) * sizeof(t_Directory *)
	);
	dir->sub_dir_count--;
	return (true);
}

t_Directory	*directory_find_sub_directory(t_Directory *dir, char *path)
{
	char	*_d_name;
	size_t	_i;

	if (NULL == dir || NULL == path)
		return (NULL);
	_i = 0;
	while (_i < dir->sub_dir_count)
	{
		if (NULL != dir->sub_directory[_i])
		{
			if (NULL == dir->sub_directory[_i]->absolute_path)
			{
				_i++;
				continue;
			}
			if (strcmp(dir->sub_directory[_i]->absolute_path, path) == 0)
				return (dir->sub_directory[_i]);
		}
		_i++;
	}
	return (NULL);
}

bool		directory_sub_directory_move(t_Directory *src, t_Directory *dst, t_Directory *sub_dir)
{
	if (NULL == src || NULL == dst || NULL == sub_dir)
		return (false);
	if (false == directory_sub_directory_remove(src, sub_dir))
		return (false);
	if (false == directory_sub_directory_add(dst, sub_dir))
		return (false);
	if (false == update_sub_directory_path(dst, sub_dir))
		return (false);
	return (true);
}

bool		directory_contains_sub_directory(t_Directory *dir, t_Directory *sub_dir)
{
	size_t	_i;

	if (NULL == dir || NULL == sub_dir)
		return (false);
	_i = 0;
	while (_i < dir->sub_dir_count)
	{
		if (sub_dir == dir->sub_directory[_i])
			return (true);
		_i++;
	}
	return (false);
}
