#ifndef SEED_FILESYSTEM_COMMANDS_H
# define SEED_FILESYSTEM_COMMANDS_H

# include "core/manager.h"

// +===----- Directory -----===+ //

/**
 * @brief Create a directory.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	fs_directory_create(t_Manager *manager, const t_Command *cmd);

/**
 * @brief Delete a directory.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	fs_directory_delete(t_Manager *manager, const t_Command *cmd);

/**
 * @brief Move a directory.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	fs_directory_move(t_Manager *manager, const t_Command *cmd);

// +===----- Files -----===+ //

/**
 * @brief Create a file.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	fs_file_create(t_Manager *manager, const t_Command *cmd);

/**
 * @brief Delete a file.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	fs_file_delete(t_Manager *manager, const t_Command *cmd);

/**
 * @brief Read a file.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	fs_file_read(t_Manager *manager, const t_Command *cmd);

/**
 * @brief Edit the data of a file.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	fs_file_edit_data(t_Manager *manager, const t_Command *cmd);

/**
 * @brief Move a file.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	fs_file_move(t_Manager *manager, const t_Command *cmd);

#endif