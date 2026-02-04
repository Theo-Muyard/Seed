#include <fcntl.h>
#include <stdlib.h>
#include "systems/filesystem/_os.h"

// +===----- OS Files -----===+ //

FILE	*os_file_create(char *path)
{
	FILE	*file;

	if (NULL == path)
		return (NULL);
	file = fopen(path, "r");
	if (NULL != file)
		return (fclose(file), NULL);
	file = fopen(path, "w");
	return (file);	
}

FILE	*os_file_open(char *path)
{
	FILE	*file;

	if (NULL == path)
		return (NULL);
	file = fopen(path, "r");
	return (file);
}

bool	*os_file_edit_path(char *old_path, char *new_path)
{
	if (NULL == old_path || NULL == new_path)
		return (false);
	if (-1 == rename(old_path, new_path))
		return (false);
	return (true);
}

bool	*os_file_edit_data(FILE *file, char *data)
{
	if (NULL == file || NULL == data)
		return (false);
	if (0 > fprintf(file,  "%s", data))
		return (false);
	return (true);
}

bool	*os_file_save(FILE *file)
{
	if (NULL == file)
		return (false);
	fclose(file);
	return (true);
}

char	*os_file_get_data(FILE *file)
{
	char	*buffer;
	size_t	_size;
	size_t	_read;

	if (NULL == file)
		return (NULL);
	fseek(file, 0, SEEK_END);
	_size = ftell(file);
	rewind(file);

	buffer = malloc((_size + 1) * sizeof(char));
	if (NULL == buffer)
		return (NULL);
	_read = fread(buffer, 1, _size, file);
	if (_size != _read)
		return (free(buffer), NULL);
	buffer[_size] = '\0';
	return (buffer);
}
