#ifndef SEED_TOOLS_MEMORY_H
# define SEED_TOOLS_MEMORY_H

// +===----- Functions -----===+ //

/**
 * @brief Duplicate and allocate the string.
 * @param str The string.
 * @return The allocated string.
*/
char	*ft_strdup(const char *str);

/**
 * 
*/
char	**ft_split(char const *s, char c);

/**
 * 
*/
void	ft_free_split(char **arr);

#endif