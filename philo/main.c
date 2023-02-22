/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zwong <zwong@student.42kl.edu.my>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 14:38:22 by zwong             #+#    #+#             */
/*   Updated: 2023/02/22 14:56:02 by zwong            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// takes in input from terminal
// set some check/flags variables
// then validate input, make sure it makes sense
t_bool	init_vars(t_vars *vars, int argc, char **argv)
{
	vars->philo_num = ft_atoi(argv[1]);
	vars->fork_num = ft_atoi(argv[1]);
	vars->time_to_die = ft_atoi(argv[2]);
	vars->time_to_eat = ft_atoi(argv[3]);
	vars->time_to_sleep = ft_atoi(argv[4]);
	vars->all_eaten = 0;
	vars->death_count = 0;
	if (vars->philo_num < 1 || vars->time_to_die < 0 || vars->time_to_eat < 0
		|| vars->time_to_sleep < 0 || vars->philo_num > 250)
		return (FALSE);
	vars->must_eat_count = -1;
	if (argc == 6)
	{
		vars->must_eat_count = atoi(argv[5]);
		if (vars->must_eat_count <= 0)
			return (FALSE);
	}
	return (TRUE);
}

// MUTEX is a locking mechanism, prevents 2 or more person to
// access memory at the same time (threads)
// Why MUTEX the forks? each fork can only be held by 1 philo at any given time
// Why MUTEX the log? to prevent messy printf in the terminal
// Why MUTEX eating? So a philo won't die while already eating (explain later)
t_bool	init_mutex(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->fork_num)
	{
		if (pthread_mutex_init(&(vars->forks[i]), NULL))
			return (FALSE);
		i++;
	}
	if (pthread_mutex_init(&(vars->log), NULL))
		return (FALSE);
	if (pthread_mutex_init(&(vars->var_change), NULL))
		return (FALSE);
	return (TRUE);
}

// God is picturing the philosophers (loading...)
// game haven't start (game starts when threads are created in start_philos())
t_bool	init_philosophers(t_vars *vars)
{
	int	i;

	i = 0;
	while (i < vars->philo_num)
	{
		vars->philos[i].id = i;
		vars->philos[i].eat_count = 0;
		vars->philos[i].left_fork = i;
		if (vars->fork_num == 1)
			vars->philos[i].right_fork = -1;
		else
		{
			vars->philos[i].right_fork
				= (vars->philo_num + i - 1) % vars->philo_num;
		}
		vars->philos[i].last_meal_ts = 0;
		vars->philos[i].vars = vars;
		if (pthread_mutex_init(&(vars->philos[i].eating), NULL))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

// main initialize necessary variables (like loading the game...)
// input argument count is either 5 or 6 only [optional: num_of_times_to_eat]
// starts running the game when initialization is complete
// NOTE: result == FALSE means somewhere in the function has failed
int	main(int argc, char **argv)
{
	t_vars	vars;

	if (argc != 5 && argc != 6)
		return (err_print("Invalid number of arguments!"));
	if (init_vars(&vars, argc, argv) == FALSE)
		return (err_print("Invalid argument values!"));
	if (init_mutex(&vars) == FALSE)
		return (err_print("Failed to initialize mutexes!"));
	if (init_philosophers(&vars) == FALSE)
		return (err_print("Failed to initialize philosophers!"));
	if (start_philos(&vars) == FALSE)
		return (err_print("Failed to initialize threads!"));
	return (0);
}
