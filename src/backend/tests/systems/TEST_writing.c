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
	_buffer = buffer_new();
	if (NULL == _buffer)
		return (printf("%sError%s: buffer_new", RED, WHITE), 1);
	printf("Buffer created %scorrectly%s.\n", GREEN, WHITE);

	// === LINE ===
	_line = line_new();
	if (NULL == _line)
		return (buffer_destroy(_buffer), printf("%sError%s: line_new", RED, WHITE), 1);
	_buffer->first_line = _line;
	printf("Line created %scorrectly%s.\n\n", GREEN, WHITE);

	// === DATA ===
	printf("No data in line:\n");
	printf("%sLine data%s: %s (%zu/%zu)\n\n", BLUE, WHITE, _line->data, _line->len,_line->capacity);
	
	printf("Adding data to the line...\n");
	if (false == add_to_line(_line, 0, 21, "salut tout le monde."))
		return (buffer_destroy(_buffer), printf("%sError%s: add_to_line", RED, WHITE), 1);
	printf("%sLine data%s: %s (%zu/%zu)\n\n", BLUE, WHITE, _line->data, _line->len,_line->capacity);

	printf("Deleting data to the line...\n");
	if (false == delete_to_line(_line, 5, 100))
		return (buffer_destroy(_buffer), printf("Error: delete_to_line"), 1);
	printf("%sLine data%s: %s (%zu/%zu)\n", BLUE, WHITE, _line->data, _line->len,_line->capacity);

	printf("\nMultiple lines...\n");

	// === Add lines ===
	_data[0] = '\0';
	_i = 2;
	while (_i < 5)
	{
		_line = line_new();
		snprintf(_data, sizeof(_data), "test numero %d", _i);
		if (NULL == _line)
			return (buffer_destroy(_buffer), printf("%sError%s: line_new", RED, WHITE), 1);
		if (false == add_to_line(_line, 0, 14, _data))
			return (buffer_destroy(_buffer), printf("%sError%s: add_to_line", RED, WHITE), 1);
		line_add_back(_buffer, _line);
		_i++;
	}
	_line = _buffer->first_line;
	while (_line)
	{
		printf("%sLine%s : %s\n", BLUE, WHITE, _line->data);
		_line = _line->next;
	}

	//  Delete line
	printf("\nDelete lines...\n");
	line_destroy(_buffer, _buffer->first_line->next);
	line_destroy(_buffer, _buffer->first_line->next->next);
	_line = _buffer->first_line;
	while (_line)
	{
		printf("%sLine%s : %s\n", BLUE, WHITE, _line->data);
		_line = _line->next;
	}

	// === Free ===
	buffer_destroy(_buffer);
	return (0);
}
