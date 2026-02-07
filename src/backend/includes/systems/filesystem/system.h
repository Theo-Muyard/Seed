#ifndef SEED_FILESYSTEM_SYSTEM_H
# define SEED_FILESYSTEM_SYSTEM_H

# include "dependency.h"

// +===----- Types -----===+ //

typedef struct s_CommandEntry	t_CommandEntry;
typedef struct s_Manager		t_Manager;
typedef struct s_Directory		t_Directory;

typedef struct s_FileSystemCtx
{
	t_Directory	*root;
	char		*root_path;
	size_t		path_len;
}	t_FileSystemCtx;

// +===----- Commands -----===+ //

# define FS_COMMANDS_COUNT 10

// TODO: Decommenter
// extern const t_CommandEntry	fs_commands[];

// +===----- Functions -----===+ //

/**
 * @brief Initialize the filesystem system.
 * @param manager The seed core manager.
 * @return TRUE for success or FALSE if an error occured.
*/
bool	fs_init(t_Manager	*manager);

/**
 * @brief Clean the writing system.
 * @param ctx The filesystem context.
*/
void	fs_clean(t_FileSystemCtx *ctx);

#endif