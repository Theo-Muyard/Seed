#ifndef SEED_WRITING_H
#define SEED_WRITING_H

# include "stddef.h"
# include "stdbool.h"

typedef struct	s_Line
{
	char			*data;
	size_t			len;
	size_t			capacity;
	struct s_Line	*prev;
	struct s_Line	*next;
}	t_Line;

typedef struct s_Buffer
{
	t_Line	*first_line;
	size_t	size;
}	t_Buffer;

/*
==============================================================
                            BUFFER
==============================================================
*/

/**
 * @brief Creates a new empty buffer.
 * @return The buffer that has just been created.
*/
t_Buffer	*buffer_new(void);

/**
 * @brief Destroys the given buffer.
 * @param buffer The buffer that will be destroyed.
*/
void		buffer_destroy(t_Buffer *buffer);

/*
==============================================================
                            LINES
==============================================================
*/

/**
 * @brief Creates a new empty line.
 * @return The line that has just been created.
*/
t_Line		*line_new(void);

/**
 * @brief Destroys the given line.
 * @param line The line that will be destroyed.
*/
void		line_destroy(t_Line *line);

/**
 * @brief Splits the given line in two lines.
 * @param line The line that will be split.
 * @param index The index where the line will be splited.
 * @return TRUE for success or FALSE if an error occured.
*/
t_Line		*line_split(t_Line *line, size_t index);

/**
 * @brief Joins the givens line in one line.
 * @param first The first line.
 * @param second The second line.
 * @return TRUE for success or FALSE if an error occured.
*/
t_Line		*line_join(t_Line *first, t_Line *second);

/*
==============================================================
                            DATA
==============================================================
*/

/**
 * @brief Add the data to the given line.
 * @param line The line.
 * @param start_col The first column where data is added.
 * @param size The size of the data.
 * @param data The data to add.
 * @return TRUE for success or FALSE if an error occured.
*/
bool		add_to_line(t_Line *line, size_t start_col, size_t size, const char *data);

/**
 * @brief Delete the data to the given line.
 * @param line The line.
 * @param start_col The first column where data is added.
 * @param size The size of the data.
 * @return TRUE for success or FALSE if an error occured.
*/
bool		delete_to_line(t_Line *line, size_t start_col, size_t size);

#endif