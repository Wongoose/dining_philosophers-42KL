/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zwong <zwong@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 14:38:37 by zwong             #+#    #+#             */
/*   Updated: 2023/02/22 14:38:38 by zwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int		i;
	long	sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = ((str[i] - 48) + (result * 10));
		i++;
	}
	return ((int)(result * sign));
}

long	timestamp(void)
{
	struct timeval	instance;

	gettimeofday(&instance, NULL);
	return ((instance.tv_sec * 1000) + (instance.tv_usec / 1000));
}

void	acknowledge_death(t_vars *vars, int id)
{
	philo_print(vars, id, "died");
	pthread_mutex_lock(&(vars->var_change));
	vars->death_count++;
	pthread_mutex_unlock(&(vars->var_change));
}

void	delay(long duration, t_vars *vars)
{
	long	i;

	i = timestamp();
	pthread_mutex_lock(&(vars->var_change));
	if (vars->death_count)
	{
		pthread_mutex_unlock(&(vars->var_change));
		return ;
	}
	pthread_mutex_unlock(&(vars->var_change));
	while (1)
	{
		if (timestamp() - i >= duration)
			break ;
		usleep(50);
	}
}

t_bool	check_all_ate(t_vars *vars, t_philo *philos)
{
	int	i;

	i = 0;
	while (vars->must_eat_count != -1 && i < vars->philo_num)
	{
		pthread_mutex_lock(&(philos[i].eating));
		if (philos[i].eat_count < vars->must_eat_count)
		{
			pthread_mutex_unlock(&(philos[i].eating));
			break ;
		}
		pthread_mutex_unlock(&(philos[i].eating));
		i++;
	}
	if (i == vars->philo_num)
		return (TRUE);
	return (FALSE);
}
