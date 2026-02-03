#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "systems/filesystem/_internal.h"

#define FILES_ALLOC	32
#define	DIR_ALLOC	32

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
	size_t	_i;

	if (NULL == dir || NULL == file)
		return (false);
	if (NULL == dir->files)
	{
		dir->files = malloc(FILES_ALLOC * sizeof(t_File));
		if (NULL == dir->files)
			return (false);
		bzero(dir->files, FILES_ALLOC * sizeof(t_File *));
		dir->files_capacity = FILES_ALLOC;
	}
	if (dir->files_count >= dir->files_capacity)
	{
		_tmp = realloc(
			dir->files,
			(FILES_ALLOC + dir->files_capacity) * sizeof(t_File)
		);
		if (NULL == _tmp)
			return (false);
		dir->files = _tmp;
		dir->files_capacity += FILES_ALLOC;
		bzero(
			*(dir->files + dir->files_count),
			FILES_ALLOC * sizeof(t_File *)
		);
	}
	_i = 0;
	while (_i < dir->files_capacity && dir->files[_i])
		_i++;
	dir->files[_i] = file;
	dir->files_count++;
	return (true);
}

bool		directory_file_remove(t_Directory *dir, t_File *file)
{
	size_t	_i;

	if (NULL == dir || NULL == file)
		return (false);
	_i = 0;
	while (_i < dir->files_capacity && dir->files[_i] != file)
		_i++;
	if (dir->files[_i] != file)
		return (false);
	dir->files[_i] = NULL;
	dir->files_count--;
	return (true); 
}

t_File		*directory_find_file(t_Directory *dir, char *filename)
{
	char	*_f_name;
	size_t	_i;

	if (NULL == dir || NULL == filename)
		return (NULL);
	_i = 0;
	while (_i < dir->files_capacity)
	{
		if (NULL != dir->files[_i])
		{
			if (NULL == dir->files[_i]->absolute_path)
				continue;
			_f_name = strrchr(dir->files[_i]->absolute_path, '/') + 1;
			if (strcmp(_f_name, filename) == 0)
				return (dir->files[_i]);
		}
		_i++;
	}
	return (NULL);
}

bool		directory_file_move(t_Directory *src, t_Directory *dst, t_File *file)
{
	t_File	*_tmp;

	if (NULL == src || NULL == dst || NULL == file)
		return (false);
	_tmp = file;
	if (false == directory_file_remove(src, file))
		return (false);
	if (false == directory_file_add(dst, _tmp))
		return (false);
	return (true);
}

bool		directory_contains_file(t_Directory *dir, t_File *file)
{
	size_t	_i;

	if (NULL == dir || NULL == file)
		return (false);
	_i = 0;
	while (_i < dir->files_capacity)
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
	size_t		_i;

	if (NULL == dir || NULL == sub_dir)
		return (false);
	if (NULL == dir->sub_directory)
	{
		dir->sub_directory = malloc(DIR_ALLOC * sizeof(t_Directory));
		if (NULL == dir->sub_directory)
			return (false);
		bzero(dir->sub_directory, DIR_ALLOC * sizeof(t_Directory *));
		dir->sub_dir_capacity = DIR_ALLOC;
	}
	if (dir->sub_dir_count >= dir->sub_dir_capacity)
	{
		_tmp = realloc(
			dir->sub_directory,
			DIR_ALLOC * sizeof(t_Directory)
		);
		if (NULL == _tmp)
			return (false);
		dir->sub_directory = _tmp;
		dir->sub_dir_capacity += DIR_ALLOC;
		bzero(
			*(dir->sub_directory + dir->sub_dir_count),
			DIR_ALLOC * sizeof(t_Directory *)
		);
	}
	_i = 0;
	while (_i < dir->sub_dir_capacity && dir->sub_directory[_i])
		_i++;
	dir->sub_directory[_i] = sub_dir;
	dir->sub_dir_count++;
	return (true);
}

bool		directory_sub_directory_remove(t_Directory *dir, t_Directory *sub_dir)
{
	size_t	_i;

	if (NULL == dir || NULL == sub_dir)
		return (false);
	_i = 0;
	while (_i < dir->sub_dir_capacity && dir->sub_directory[_i] != sub_dir)
		_i++;
	if (dir->sub_directory[_i] != sub_dir)
		return (false);
	dir->sub_directory[_i] = NULL;
	dir->sub_dir_count--;
	return (true);
}

t_Directory	*directory_find_sub_directory(t_Directory *dir, char *dirname)
{
	char	*_d_name;
	size_t	_i;

	if (NULL == dir || NULL == dirname)
		return (NULL);
	_i = 0;
	while (_i < dir->sub_dir_capacity)
	{
		if (NULL != dir->sub_directory[_i])
		{
			if (NULL == dir->sub_directory[_i]->absolute_path)
				continue;
			_d_name = strrchr(dir->sub_directory[_i]->absolute_path, '/') + 1;
			if (strcmp(_d_name, dirname) == 0)
				return (dir->sub_directory[_i]);
		}
		_i++;
	}
	return (NULL);
}

bool		directory_sub_directory_move(t_Directory *src, t_Directory *dst, t_Directory *sub_dir)
{
	t_Directory	*_tmp;

	if (NULL == src || NULL == dst || NULL == sub_dir)
		return (false);
	_tmp = sub_dir;
	if (false == directory_sub_directory_remove(src, sub_dir))
		return (false);
	if (false == directory_sub_directory_add(dst, _tmp))
		return (false);
	return (true);
}

bool		directory_contains_sub_directory(t_Directory *dir, t_Directory *sub_dir)
{
	size_t	_i;

	if (NULL == dir || NULL == sub_dir)
		return (false);
	_i = 0;
	while (_i < dir->sub_dir_capacity)
	{
		if (sub_dir == dir->sub_directory[_i])
			return (true);
		_i++;
	}
	return (false);
}
