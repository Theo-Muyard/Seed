#include <stdio.h>
#include "writing.h"

#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define BLUE	"\033[34m"
#define WHITE	"\033[37m"

int	main(void)
{
	t_Buffer	*_buffer;
	t_Line		*_line;
	char		_data[50];
	int			_i;

	// === BUFFER ===
	_buffer = buffer_create();
	if (NULL == _buffer)
		return (printf("%sError%s: buffer_create\n", RED, WHITE), 1);
	printf("Buffer created %scorrectly%s.\n", GREEN, WHITE);

	// === LINE ===
	_line = line_create();
	if (NULL == _line)
		return (buffer_destroy(_buffer), printf("%sError%s: line_create\n", RED, WHITE), 1);
	if (false == buffer_line_insert(_buffer, _line, 0))
		return (buffer_destroy(_buffer), printf("%sError%s: buffer_line_insert\n", RED, WHITE), 1);
	printf("Line created %scorrectly%s.\n\n", GREEN, WHITE);

	// === DATA ===
	printf("No data in line:\n");
	printf("%sLine data%s: %s (%zu/%zu)\n\n", BLUE, WHITE, _line->data, _line->len,_line->capacity);
	
	printf("Adding data to the line...\n");
	if (false == line_add_data(_line, 0, 21, "salut tout le monde."))
		return (buffer_destroy(_buffer), printf("%sError%s: line_add_data\n", RED, WHITE), 1);
	printf("%sLine data%s: %s (%zu/%zu)\n\n", BLUE, WHITE, _line->data, _line->len,_line->capacity);

	printf("Deleting data to the line...\n");
	if (false == line_delete_data(_line, 5, 100))
		return (buffer_destroy(_buffer), printf("Error: line_delete_data\n"), 1);
	printf("%sLine data%s: %s (%zu/%zu)\n", BLUE, WHITE, _line->data, _line->len,_line->capacity);

	printf("\nMultiple lines...\n");

	// === Add lines ===
	_data[0] = '\0';
	_i = 2;
	while (_i < 6)
	{
		_line = line_create();
		snprintf(_data, sizeof(_data), "test numero %d", _i);
		if (NULL == _line)
			return (buffer_destroy(_buffer), printf("%sError%s: line_create\n", RED, WHITE), 1);
		if (false == line_add_data(_line, 0, 14, _data))
			return (buffer_destroy(_buffer), printf("%sError%s: line_add_data\n", RED, WHITE), 1);
		buffer_line_insert(_buffer, _line, -1);
		_i++;
	}
	_line = _buffer->line;
	while (_line)
	{
		printf("%sLine%s : %s\n", BLUE, WHITE, _line->data);
		_line = _line->next;
	}

	// === Delete lines ===
	printf("\nDelete lines...\n");
	buffer_line_destroy(_buffer, _buffer->line->next);
	buffer_line_destroy(_buffer, _buffer->line->next);
	_line = _buffer->line;
	while (_line)
	{
		printf("%sLine%s : %s\n", BLUE, WHITE, _line->data);
		_line = _line->next;
	}

	// === Join lines ===
	printf("\nJoin lines...\n");
	if (NULL == buffer_line_join(_buffer, _buffer->line, _buffer->line->next))
		return (buffer_destroy(_buffer), printf("%sError%s: buffer_line_join\n", RED, WHITE), 1);
	_line = _buffer->line;
	while (_line)
	{
		printf("%sLine%s : %s\n", BLUE, WHITE, _line->data);
		_line = _line->next;
	}
	// === Split lines ===
	printf("\nSplit lines...\n");
	if (NULL == buffer_line_split(_buffer, _buffer->line, 5))
		return (buffer_destroy(_buffer), printf("%sError%s: buffer_line_split\n", RED, WHITE), 1);
	if (NULL == buffer_line_split(_buffer, _buffer->line->next, 2))
		return (buffer_destroy(_buffer), printf("%sError%s: buffer_line_split\n", RED, WHITE), 1);
	_line = _buffer->line;
	while (_line)
	{
		printf("%sLine%s : %s\n", BLUE, WHITE, _line->data);
		_line = _line->next;
	}

	// === Add line ===
	printf("\nAdding line...\n");
	_line = line_create();
	if (NULL == _line)
		return (buffer_destroy(_buffer), printf("%sError%s: line_create\n", RED, WHITE), 1);
	buffer_line_insert(_buffer, _line, 3);
	line_add_data(_line, 0, 8, "un test");
	_line = _buffer->line;
	while (_line)
	{
		printf("%sLine%s : %s\n", BLUE, WHITE, _line->data);
		_line = _line->next;
	}

	// === Free ===
	buffer_destroy(_buffer);
	return (0);
}
