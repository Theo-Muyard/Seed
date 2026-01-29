#ifndef SEED_WRITING_SYSTEM_H
# define SEED_WRITING_SYSTEM_H

# include <stdbool.h>

// +===----- Types -----===+ //

typedef struct s_CommandEntry	t_CommandEntry;
typedef struct s_Manager		t_Manager;
typedef struct s_Buffer			t_Buffer;

typedef struct s_WritingCtx
{
	t_Buffer	**buffers;
	size_t		count;
	size_t		capacity;
}	t_WritingCtx;

// +===----- Commands -----===+ //

const t_CommandEntry	writing_commands[] = {
	{ CMD_WRITING_CREATE_BUFFER,	cmd_buffer_create},
	{ CMD_WRITING_DELETE_BUFFER,	cmd_buffer_destroy},

	{ CMD_WRITING_INSERT_LINE,		cmd_buffer_line_insert},
	{ CMD_WRITING_DELETE_LINE,		cmd_buffer_line_destroy},
	{ CMD_WRITING_SPLIT_LINE,		cmd_buffer_line_split},
	{ CMD_WRITING_JOIN_LINE,		cmd_buffer_line_join},
	{ CMD_WRITING_GET_LINE,			cmd_buffer_get_line},

	{ CMD_WRITING_INSERT_TEXT,		cmd_line_add_data},
	{ CMD_WRITING_DELETE_TEXT,		cmd_line_delete_data},
};

// +===----- Functions -----===+ //

/**
 * @brief Initialize the writing system.
 * @param manager The seed core manager.
 * @return TRUE for success or FALSE if an error occured.
*/
bool	writing_init(t_Manager	*manager);

/**
 * @brief Clean the writing system.
 * @param manager The seed core manager.
*/
void	writing_clean(t_Manager	*manager);

#endif