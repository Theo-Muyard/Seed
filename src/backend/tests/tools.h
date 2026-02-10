#ifndef SEED_TESTS_TOOLS_H
# define SEED_TESTS_TOOLS_H

# include "dependency.h"

// +===----- Colors -----===+ //

#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define WHITE	"\033[37m"

// +===----- Test Utilities -----===+ //

/**
 * @brief Print a section message.
 * @param section The name of the section.
*/
void	print_section(const char *section);

/**
 * @brief Print a success message.
 * @param message The success message.
*/
void	print_success(const char *message);

/**
 * @brief Print an error message.
 * @param message The error message.
*/
void	print_error(const char *message);

/**
 * @brief Print the final status.
 * @param status The status of the test (0 or 1).
*/
void	print_status(int status);

#endif