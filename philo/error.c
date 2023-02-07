#include "philo.h"

int	err_print(char *message)
{
	write(2, "Error\n", 6);
	while (*message)
		write(2, message++, 1);
	return (1);
}
