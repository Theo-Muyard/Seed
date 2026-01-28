#ifndef SEED_WRITING_SYSTEM_H
# define SEED_WRITING_SYSTEM_H

# include <sys/types.h>
# include <stdbool.h>
# include "core/manager.h"
# include "systems/writing/internal.h"

// +===----- Types -----===+ //

typedef struct	s_writingCtx
{
	t_Buffer	**buffers;
	size_t		count;
	size_t		capacity;
}	t_writingCtx;

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
 * @return TRUE for success or FALSE if an error occured.
*/
bool	writing_clean(t_Manager	*manager);

#endif