#ifndef SEED_FILESYSTEM_COMMANDS_H
# define SEED_FILESYSTEM_COMMANDS_H

# include "core/manager.h"

// +===----- Macros -----===+ //

/**
 * @brief Test if the os dir function return an error and catch it
 * @param fn The os function
*/
# define TEST_OS_DIR_ERR(fn)	\
	if (false == fn)	\
		return (get_dir_error());

/**
 * @brief Test if the os file function return an error and catch it
 * @param fn The os function
*/
# define TEST_OS_FILE_ERR(fn)	\
	if (false == fn)	\
		return (get_file_error());

// +===----- OS Errors -----===+ //

/**
 * @brief Get the file error.
 * @return The error.
*/
t_ErrorCode	get_file_error(void);

/**
 * @brief Get the dir error.
 * @return The error.
*/
t_ErrorCode	get_dir_error(void);

// +===----- Root -----===+ //

/**
 * @brief Create a directory.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	cmd_root_open(t_Manager *manager, const t_Command *cmd);

/**
 * @brief Create a directory.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	cmd_root_close(t_Manager *manager, const t_Command *cmd);


// +===----- Directory -----===+ //

/**
 * @brief Create a directory.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	cmd_directory_create(t_Manager *manager, const t_Command *cmd);

/**
 * @brief Delete a directory.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	cmd_directory_delete(t_Manager *manager, const t_Command *cmd);

/**
 * @brief Move a directory.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	cmd_directory_move(t_Manager *manager, const t_Command *cmd);

// +===----- Files -----===+ //

/**
 * @brief Create a file.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	cmd_file_create(t_Manager *manager, const t_Command *cmd);

/**
 * @brief Delete a file.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	cmd_file_delete(t_Manager *manager, const t_Command *cmd);

/**
 * @brief Move a file.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	cmd_file_move(t_Manager *manager, const t_Command *cmd);

/**
 * @brief Read a file.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	cmd_file_read(t_Manager *manager, const t_Command *cmd);

/**
 * @brief Edit the data of a file.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	cmd_file_edit_data(t_Manager *manager, const t_Command *cmd);

#endif