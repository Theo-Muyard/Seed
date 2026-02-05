#ifndef SEED_MANAGER_H
# define SEED_MANAGER_H

# include <sys/types.h>
# include <stdbool.h>

// +===----- Types -----===+ //

typedef enum e_ErrorCode		t_ErrorCode;
typedef struct s_Command		t_Command;
typedef struct s_Dispatcher		t_Dispatcher;
typedef struct s_WritingCtx		t_WritingCtx;
typedef struct s_FileSystemCtx	t_FileSystemCtx;

typedef struct	s_Manager
{
	t_Dispatcher		*dispatcher;
	t_WritingCtx		*writing_ctx;
	t_FileSystemCtx		*fs_ctx;
}	t_Manager;

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