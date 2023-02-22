/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zwong <zwong@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 14:38:17 by zwong             #+#    #+#             */
/*   Updated: 2023/02/22 14:38:18 by zwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	err_print(char *message)
{
	write(2, "Error\n", 6);
	while (*message)
		write(2, message++, 1);
	return (1);
}

// Note: "\n" newline is needed to printf during realtime, else all
// log will be stored in buffer until program completes
void	philo_print(t_vars *vars, unsigned int id, char *message)
{
	pthread_mutex_lock(&(vars->log));
	pthread_mutex_lock(&(vars->var_change));
	if (!vars->death_count)
	{
		pthread_mutex_unlock(&(vars->var_change));
		printf("%li ", timestamp() - vars->init_timestamp);
		printf("%u ", id + 1);
		printf("%s\n", message);
	}
	pthread_mutex_unlock(&(vars->var_change));
	pthread_mutex_unlock(&(vars->log));
}
