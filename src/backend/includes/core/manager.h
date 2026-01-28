#ifndef SEED_MANAGER_H
# define SEED_MANAGER_H

# include <sys/types.h>
# include <stdbool.h>

// +===----- Types -----===+ //

typedef t_Dispatch		t_Dispatch;
typedef t_WritingCtx	t_WritingCtx;

typedef struct	s_Manager
{
	t_Dispatch		*dispatch;
	t_WritingCtx	*writing_ctx;
}	t_Manager;

// +===----- Functions -----===+ //

#endif