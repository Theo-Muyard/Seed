#include "writing.h"
#include "commands.h"

/**
 * @brief Get the line of the command.
 * @param buffer The buffer that will contains lines.
 * @param cmd The command content.
*/
static t_Line	*command_writing_get_line(t_Buffer *buffer, t_Command *cmd);

/**
 * @brief Execute the command of writing system. 
 * @param buffer The buffer that will be contains the lines.
 * @param cmd The command content to execute.
*/
static bool		command_writing_exec(t_Buffer *buffer, t_Command *cmd);



bool	command_exec(void *system_ctx, t_Command *cmd);
