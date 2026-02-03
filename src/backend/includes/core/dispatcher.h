#ifndef SEED_DISPATCHER_H
# define SEED_DISPATCHER_H

# include <sys/types.h>
# include <stdbool.h>
# include "seed.h"

// +===----- Types -----===+ //

typedef struct s_Command	t_Command;

typedef t_ErrorCode	(*t_Fn)(t_Manager *manager, const t_Command *cmd);

typedef struct	s_CommandEntry
{
	t_CommandId	id;
	size_t		size;
	t_Fn		fn;
}	t_CommandEntry;

#define TYPE_DESC(type) \
	(&(t_PayloadTypeDesc){.size = sizeof(type), .name = #type})

typedef struct s_Dispatcher
{
	size_t			count;
	size_t			capacity;
	t_CommandEntry	*commands;
}	t_Dispatcher;

// +===----- Functions -----===+ //

/**
 * @brief Register the command with his function.
 * @param manager The manager that will contains contexts.
 * @param capacity The number of commands that will be contains.
 * @return TRUE for success or FALSE if an error occured.
*/
bool	dispatcher_init(t_Manager *manager, size_t capacity);

/**
 * @brief Clean the dispatcher.
 * @param dispatcher The dispatcher that will contains commands.
*/
void	dispatcher_clean(t_Dispatcher *dispatcher);

/**
 * @brief Register the command with his function.
 * @param dispatcher The dispatcher that will contains commands.
 * @param id The id of the command.
 * @param size The size of the payload type.
 * @param function The function to execute for the given command.
 * @return TRUE for success or FALSE if an error occured.
*/
bool	dispatcher_register(
	t_Dispatcher *dispatcher,
	t_CommandId id,
	size_t size,
	t_Fn fn
);

/**
 * @brief Execute the function of the specified command.
 * @param manager The manager that will contains contexts.
 * @param cmd The content of the command.
 * @return An error code or SUCCESS (=0).
*/
t_ErrorCode	dispatcher_exec(t_Manager *manager, const t_Command *cmd);

#endif