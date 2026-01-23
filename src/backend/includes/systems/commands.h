#ifndef SEED_COMMANDS_H
# define SEED_COMMANDS_H

# include <unistd.h>
# include <stdbool.h>

// +===----- ENUM -----===+ //

typedef enum e_CommandType
{
	CMD_CREATE_BUFFER,	// COMMAND: BUFFER
	CMD_DELETE_BUFFER,
	CMD_CREATE_LINE,	// COMMAND: LINE
	CMD_INSERT_LINE,
	CMD_SPLIT_LINE,
	CMD_JOIN_LINE,
	CMD_DELETE_LINE,
	CMD_INSERT_TEXT,	// COMMAND: TEXT
	CMD_DELETE_TEXT,
}	t_CommandType;

// +===----- TYPES -----===+ //

typedef struct s_Command
{
	t_CommandType	type;
	size_t			line;
	size_t			column;
	size_t			size;
	const char		*data;
}	t_Command;

typedef	bool (*t_Command_exec_fn)(void *, t_Command *);

typedef struct s_CommandMap
{
	t_CommandType 		type;
	t_Command_exec_fn	fn;
}	t_CommandMap;

static t_CommandMap command_map[] = {
	{ CMD_CREATE_BUFFER,	command_writing_exec },	// COMMAND: BUFFER
	{ CMD_DELETE_BUFFER,	command_writing_exec },
	{ CMD_CREATE_LINE,		command_writing_exec },	// COMMAND: LINE
	{ CMD_INSERT_LINE,		command_writing_exec },
	{ CMD_SPLIT_LINE,		command_writing_exec },
	{ CMD_JOIN_LINE,		command_writing_exec },
	{ CMD_DELETE_LINE,		command_writing_exec },
	{ CMD_INSERT_TEXT,		command_writing_exec },	// COMMAND: TEXT
	{ CMD_DELETE_TEXT,		command_writing_exec },
};

typedef struct s_Buffer	t_Buffer;


// +===----- FUNCTION -----===+ //

bool	command_exec(void *system_ctx, t_Command *cmd);

#endif