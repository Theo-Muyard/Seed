#ifndef SEED_FILESYSTEM_INTERNAL_H
# define SEED_FILESYSTEM_INTERNAL_H

# include <sys/types.h>
# include <stdbool.h>

// +===----- Types -----===+ //

typedef struct	s_Directory
{
	t_File		**files;
	size_t		files_count;
	size_t		files_capacity;
	t_Directory	**sub_directory;
	size_t		sub_dir_count;
	size_t		sub_dir_capacity;
}	t_Directory;

typedef struct	s_File
{
	ssize_t	buffer_id;
	char	*absolute_path;
}	t_File;

// +===----- Directory -----===+ //

/**
 * @brief Creates a new empty directory.
 * @return The directory that has just been created.
*/
t_Directory	*directory_create(void);

/**
 * @brief Destroys the given directory.
 * @dir The directory that will be destroyed.
*/
void		directory_destroy(t_Directory *dir);

// +===----- Files -----===+ //

/**
 * @brief Creates a new empty file.
 * @return The file that has just been created.
*/
t_File		*file_create(void);

/**
 * @brief Destroys the given file.
 * @param file The file that will be destroyed.
*/
void		file_destroy(t_File *file);

/**
 * @brief Add the given file to the directory.
 * @param dir The directory that contains files.
 * @param file The file that will be added. 
 * @return TRUE for success or FALSE if an error occured.
*/
bool		directory_add_file(t_Directory *dir, t_File *file);

/**
 * @brief Remove the given file.
 * @param dir The directory that contains files.
 * @param file The file that will be removed.
 * @return TRUE for success or FALSE if an error occured.
*/
bool		directory_file_remove(t_Directory *dir, t_File *file);

/**
 * @brief Find a file by its name in the given directory.
 * @param dir The directory that contains files and sub directory.
 * @param filename The name of the file.
 * @return The file or NULL if not found.
*/
t_File		*directory_find_file(t_Directory *dir, char *filename);

/**
 * @brief Move a file to src in dst directory.
 * @param src The source directory.
 * @param dst The destination directory.
 * @param file The file that will be moved.
 * @return TRUE for success or FALSE if an error occured.
*/
bool		directory_file_move(t_Directory *src, t_Directory *dst, t_File *file);

/**
 * @brief Check if the directory contains a specific file.
 * @param dir The directory that contains files and sub directory.
 * @param file The file.
 * @return TRUE for success or FALSE if an error occured.
*/
bool		directory_contains_file(t_Directory *dir, t_File *file);

// +===----- Sub directory -----===+ //

/**
 * @brief Add the given sub directory to the directory.
 * @param dir The directory that contains files.
 * @param sub_dir The sub directory that will be added.
 * @return TRUE for success or FALSE if an error occured.
*/
bool		directory_add_sub_directory(t_Directory *dir, t_Directory *sub_dir);

/**
 * @brief Remove the given sub directory.
 * @param dir The directory that contains files.
 * @param file The sub_directory that will be removed.
 * @return TRUE for success or FALSE if an error occured.
*/
bool		directory_remove_sub_directory(t_Directory *dir, t_Directory *sub_dir);

/**
 * @brief Find a sub directory by its name in the given directory.
 * @param dir The directory that contains files and sub directory.
 * @param dirname The name of the sub directory.
 * @return The file or NULL if not found.
*/
t_File		*directory_find_sub_directory(t_Directory *dir, char *dirname);

/**
 * @brief Move a sub directory to src in dst directory.
 * @param src The source directory.
 * @param dst The destination directory.
 * @param sub_dir The sub directory that will be moved.
 * @return TRUE for success or FALSE if an error occured.
*/
bool		directory_sub_directory_move(t_Directory *src, t_Directory *dst, t_Directory *sub_dir);

/**
 * @brief Check if the directory contains a specific sub directory.
 * @param dir The directory that contains files and sub directory.
 * @param sub_dir The file.
 * @return TRUE for success or FALSE if an error occured.
*/
bool		directory_contains_sub_directory(t_Directory *dir, t_Directory *sub_dir);

#endif