#include <stdlib.h>
#include <string.h>
#include "writing.h"

#define DATA_REALLOC 32

/*
==============================================================
                            BUFFER
==============================================================
*/

t_Buffer	*buffer_new(void)
{
	t_Buffer	*buffer;

	buffer = malloc(sizeof(t_Buffer));
	if (NULL == buffer)
		return (NULL);
	buffer->first_line = NULL;
	buffer->size = 0;
	return (buffer);
}

void		buffer_destroy(t_Buffer *buffer)
{
	t_Line	*_tmp;

	if (NULL == buffer)
		return ;
	while (buffer->first_line)
	{
		_tmp = buffer->first_line;
		line_destroy(buffer->first_line);
		buffer->first_line = _tmp->next;
	}
	free(buffer);
}

/*
==============================================================
                            LINES
==============================================================
*/

t_Line		*line_new(void)
{
	t_Line	*line;

	line = malloc(sizeof(t_Line));
	if (NULL == line)
		return (NULL);
	line->data = NULL;
	line->len = 0;
	line->capacity = 0;
	line->prev = NULL;
	line->next = NULL;
	return (line);
}

void		line_destroy(t_Line *line)
{
	if (NULL == line)
		return ;
	free(line->data);
	free(line);
}

/*
==============================================================
                            DATA
==============================================================
*/

bool		add_to_line(t_Line *line, size_t start_col, size_t size, const char *data)
{
	char	*_new_data;
	size_t	_needed_capacity;
	size_t	_new_capacity;

	if (NULL == line || NULL == data)
		return (false);
	if (start_col > line->len)
		return (false);

	_needed_capacity = line->len + size;
	if (_needed_capacity > line->capacity)
	{
		_new_capacity = line->capacity;
		while (_new_capacity < _needed_capacity)
			_new_capacity += DATA_REALLOC;
		_new_data = realloc(line->data, _new_capacity * sizeof(char));
		if (NULL == _new_data)
			return (false);
		line->data = _new_data;
		line->capacity = _new_capacity;
	}
	memmove(
		line->data + start_col + size,
		line->data + start_col,
		line->len - start_col
	);
	memcpy(line->data + start_col, data, size);
	line->len += size;
	return (true);
}

bool		delete_to_line(t_Line *line, size_t start_col, size_t size)
{
	size_t	_new_capacity;

	if (NULL == line)
		return (false);
	if (start_col > line)
		return (false);
}
