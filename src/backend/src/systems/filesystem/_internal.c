#include <stdlib.h>
#include <strings.h>
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
	int _i;

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

bool		directory_add_file(t_Directory *dir, t_File *file)
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
	file_destroy(file);
	dir->files[_i] = NULL;
	dir->files_count--;
	return (true); 
}

bool		directory_add_sub_directory(t_Directory *dir, t_Directory *sub_dir)
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

bool		directory_remove_sub_directory(t_Directory *dir, t_Directory *sub_dir)
{
	size_t	_i;

	if (NULL == dir || NULL == sub_dir)
		return (false);
	_i = 0;
	while (_i < dir->sub_dir_capacity && dir->sub_directory[_i] != sub_dir)
		_i++;
	if (dir->sub_directory[_i] != sub_dir)
		return (false);
	directory_destroy(sub_dir);
	dir->sub_directory[_i] = NULL;
	dir->sub_dir_count--;
	return (true);
}

