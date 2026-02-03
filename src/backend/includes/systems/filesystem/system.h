#ifndef SEED_FILESYSTEM_SYSTEM_H
# define SEED_FILESYSTEM_SYSTEM_H

# include <stdbool.h>
# include <sys/types.h>

// +===----- Types -----===+ //

typedef struct s_CommandEntry	t_CommandEntry;
typedef struct s_Manager		t_Manager;
typedef struct s_Buffer			t_Buffer;

// TODO: Changer pour filesystem 

typedef struct s_FileSystemCtx
{
	
}	t_FileSystemCtx;

// // +===----- Commands -----===+ //

// # define WRITING_COMMANDS_COUNT 9

// extern const t_CommandEntry	writing_commands[];

// // +===----- Functions -----===+ //

// /**
//  * @brief Initialize the writing system.
//  * @param manager The seed core manager.
//  * @return TRUE for success or FALSE if an error occured.
// */
// bool	writing_init(t_Manager	*manager);

// /**
//  * @brief Clean the writing system.
//  * @param ctx The writing context.
// */
// void	writing_clean(t_WritingCtx *ctx);

#endif