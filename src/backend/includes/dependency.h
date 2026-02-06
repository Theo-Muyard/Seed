#ifndef SEED_DEPENDENCY_H
# define SEED_DEPENDENCY_H

// +===----- Headers -----===+ //

# include <sys/types.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <errno.h>
# include <string.h>

// +===----- Macros -----===+ //

/**
 * @brief Test if the variable is NULL.
 * @param var The variable that will be tested.
 * @param err The value returned.
*/
# define TEST_NULL(var, err) \
	if (NULL == var) return (err)

/**
 * @brief Test if the function return false.
 * @param fn The function that will be tested.
 * @param err The value returned.
*/
# define TEST_ERROR_FN(fn, err) \
	if (false == fn) return (err)

#endif