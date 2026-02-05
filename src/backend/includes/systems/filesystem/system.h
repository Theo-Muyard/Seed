#ifndef SEED_FILESYSTEM_SYSTEM_H
# define SEED_FILESYSTEM_SYSTEM_H

# include <stdbool.h>
# include <sys/types.h>

// +===----- Types -----===+ //

typedef struct s_CommandEntry	t_CommandEntry;
typedef struct s_Manager		t_Manager;
typedef struct s_Directory		t_Directory;

typedef struct s_FileSystemCtx
{
	t_Directory	*root;
}	t_FileSystemCtx;

// // +===----- Commands -----===+ //

# define FS_COMMANDS_COUNT 8

extern const t_CommandEntry	fs_commands[];

// // +===----- Functions -----===+ //

/**
 * @brief Initialize the filesystem system.
 * @param manager The seed core manager.
 * @param root_path The root path.
 * @return TRUE for success or FALSE if an error occured.
*/
bool	fs_init(t_Manager	*manager, char *root_path);

/**
 * @brief Clean the writing system.
 * @param ctx The filesystem context.
*/
void	fs_clean(t_Directory *ctx);

#endif