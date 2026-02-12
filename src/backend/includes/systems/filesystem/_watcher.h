#ifndef SEED_FILESYSTEM_WATCHER_H
# define SEED_FILESYSTEM_WATCHER_H

# include "dependency.h"
# include "systems/filesystem/system.h"

// +===----- Types -----===+ //

/* A pending move */
typedef struct	s_MovePending
{
	uint32_t	cookie;	/* The OS cookie */
	bool		is_dir;	/* Directory or file */
	char		*from_path;	/* The old path */
}	t_MovePending;

// +===----- Functions -----===+ //

/**
 * @brief Initialize the VFS root. Retrive all subdirs and files.
 * @param root The root directory of the VFS.
 * @param abs_path The absolute path of the root directory.
 * @return A Seed ErrorCode.
*/
t_ErrorCode	get_VFS_root(t_Directory *root, const char *abs_path);

/**
 * @brief Apply the OS events to the VFS.
 * @param root The root directory.
 * @param inotify_fd The fd of the root.
 * @return TRUE for success or FALSE if an error occured.
*/
bool		apply_events(t_Directory *root, int inotify_fd);

#endif