#ifndef SEED_H
# define SEED_H

// +===----- Seed Backend Public API -----===+ //
// This header exposes only what the frontend needs

# include <sys/types.h>
# include <stdbool.h>

// +===----- Types -----===+ //

typedef struct s_Manager	t_Manager;

typedef enum	e_ErrorCode
{
	ERR_SUCCESS = 0,

	ERR_INTERNAL_MEMORY,					// Internal
	ERR_OPERATION_FAILED,
	ERR_PERMISSION_DENIED,

	ERR_INVALID_MANAGER,					// Invalid
	ERR_INVALID_PAYLOAD,
	ERR_INVALID_COMMAND,
	ERR_INVALID_COMMAND_ID,

	ERR_DISPATCHER_NOT_INITIALIZED,			// Writing
	ERR_WRITING_CONTEXT_NOT_INITIALIZED,
	ERR_BUFFER_NOT_FOUND,
	ERR_LINE_NOT_FOUND,

	ERR_FS_CONTEXT_NOT_INITIALIZED,			// Fs
	ERR_DIR_NOT_FOUND,
	ERR_DIR_ACCESS,
	ERR_DIR_EXIST,
	ERR_FILE_NOT_FOUND,
	ERR_FILE_ACCESS,
	ERR_FILE_EXIST
}	t_ErrorCode;

typedef enum	e_CommandId
{
	CMD_WRITING_CREATE_BUFFER,	// Writing
	CMD_WRITING_DELETE_BUFFER,

	CMD_WRITING_INSERT_LINE,
	CMD_WRITING_DELETE_LINE,
	CMD_WRITING_SPLIT_LINE,
	CMD_WRITING_JOIN_LINE,
	CMD_WRITING_GET_LINE,

	CMD_WRITING_INSERT_TEXT,
	CMD_WRITING_DELETE_TEXT,

	CMD_FS_CREATE_DIR,			// Fs
	CMD_FS_DELETE_DIR,
	CMD_FS_MOVE_DIR,

	CMD_FS_CREATE_FILE,
	CMD_FS_DELETE_FILE,
	CMD_FS_READ_FILE,
	CMD_FS_EDIT_FILE,
	CMD_FS_MOVE_FILE
}	t_CommandId;

typedef struct s_Command
{
	t_CommandId		id;
	void			*payload;
}	t_Command;

// +===----- Payload types -----===+ //

// +===----- Writing -----===+ //

typedef struct	s_CmdCreateBuffer
{
	size_t	out_buffer_id;
}	t_CmdCreateBuffer;

typedef struct	s_CmdDestroyBuffer
{
	size_t	buffer_id;
}	t_CmdDestroyBuffer;

typedef struct	s_CmdInsertLine
{
	size_t	buffer_id;
	ssize_t	line;
}	t_CmdInsertLine;

typedef struct	s_CmdDeleteLine
{
	size_t	buffer_id;
	ssize_t	line;
}	t_CmdDeleteLine;

typedef struct	s_CmdSplitLine
{
	size_t	buffer_id;
	ssize_t	line;
	size_t	index;
}	t_CmdSplitLine;

typedef struct	s_CmdJoinLine
{
	size_t	buffer_id;
	size_t	dst;
	size_t	src;
}	t_CmdJoinLine;

typedef struct	s_CmdGetLine
{
	size_t		buffer_id;
	ssize_t		line;
	const char	*out_data;
	size_t		out_len;
}	t_CmdGetLine;

typedef struct	s_CmdInsertData
{
	size_t	buffer_id;
	ssize_t	line;
	ssize_t	column;
	size_t	size;
	char	*data;
}	t_CmdInsertData;

typedef struct	s_CmdDeleteData
{
	size_t	buffer_id;
	ssize_t	line;
	size_t	column;
	size_t	size;
}	t_CmdDeleteData;

// +===----- Filesystem -----===+ //

typedef struct	s_CmdCreateDir
{
	char			*parent_dirname;
	char			*dirname;
	unsigned int	mode;
}	t_CmdCreateDir;

typedef struct	s_CmdDeleteDir
{
	char	*parent_dirname;
	char	*dirname;
}	t_CmdDeleteDir;

typedef struct	s_CmdMoveDir
{
	char	*old_path;
	char	*new_path;
}	t_CmdMoveDir;

typedef struct	s_CmdCreateFile
{
	char	*path;
	unsigned int	mode;
}	t_CmdCreateFile;

typedef struct	s_CmdDeleteFile
{
	char	*path;
}	t_CmdDeleteFile;

typedef struct	s_CmdMoveFile
{
	char	*old_path;
	char	*new_path;
}	t_CmdMoveFile;

typedef struct	s_CmdReadFile
{
	char		*path;
	const char	*out_data;
	size_t		out_len;
}	t_CmdReadFile;

typedef struct	s_CmdEditDataFile
{
	char	*path;
	char	*data;
	size_t	len;
}	t_CmdEditDataFile;

// +===----- Functions -----===+ //

/**
 * @brief Initialize the seed core manager.
 * @return The manager who was created.
*/
t_Manager	*manager_init(void);

/**
 * @brief Clean the seed core manager.
 * @param manager The manager.
*/
void		manager_clean(t_Manager *manager);

/**
 * @brief Clean the seed core manager.
 * @param manager The manager.
 * @param cmd The command content.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	manager_exec(t_Manager *manager, t_Command *cmd);

#endif