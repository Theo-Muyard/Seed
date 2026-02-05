#include <stdlib.h>
#include "tools/memory.h"

char	*ft_strdup(const char *str)
{
	int		_i;
	char	*ptr;

	if (NULL == str)
		return (NULL);
	ptr = malloc((ft_strlen(str) + 1) * sizeof(char));
	if (NULL == ptr)
		return (NULL);
	_i = 0;
	while (str[_i])
	{
		ptr[_i] = str[_i];
		_i++;
	}
	ptr[_i] = '\0';
	return (ptr);
}
