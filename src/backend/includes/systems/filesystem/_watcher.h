#ifndef SEED_FILESYSTEM_WATCHER_H
# define SEED_FILESYSTEM_WATCHER_H

# include "dependency.h"
# include "systems/filesystem/system.h"

// +===----- Types -----===+ //

typedef struct s_WatchEntry
{
	int		wd;
	char	*path;
}	t_WatchEntry;

typedef struct s_MovePending
{
	uint32_t	cookie;
	bool		is_dir;
	char		*from_path;
	time_t		created_at;
}	t_MovePending;

typedef struct s_WatchCtx
{
	int				fd;
	char			*root_path;

	t_WatchEntry	*entries;
	size_t			entry_count;
	size_t			entry_capacity;

	t_MovePending	*pending;
	size_t			pending_count;
	size_t			pending_capacity;
}	t_WatchCtx;

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