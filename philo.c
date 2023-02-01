#include "philo.h"

void	philo_eats(t_philo *philo)
{
	t_vars *vars;

	vars = philo->vars;
	pthread_mutex_lock(&(vars->forks[philo->left_fork]));
	philo_print(vars, philo->id, "has taken a fork");
	pthread_mutex_lock(&(vars->forks[philo->right_fork]));
	philo_print(vars, philo->id, "has taken a fork");
	pthread_mutex_lock(&(vars->meal_check));
	philo_print(vars, philo->id, "is eating");
	philo->last_meal_ts = timestamp();
	pthread_mutex_unlock(&(vars->meal_check));
	delay(vars->time_to_eat, vars);
	(philo->eat_count)++;
	pthread_mutex_unlock(&(vars->forks[philo->left_fork]));
	pthread_mutex_unlock(&(vars->forks[philo->right_fork]));
}

void	*philo_routine(void *data)
{
	t_philo *philo;
	t_vars *vars;

	philo = (t_philo *)data;
	vars = philo->vars;
	if (philo->id % 2)
		usleep(15000);
	while (!vars->death_count)
	{
		if (vars->all_eaten)
			break ;
		philo_eats(philo);
		philo_print(vars, philo->id, "is sleeping");
		delay(vars->time_to_sleep, vars);
		philo_print(vars, philo->id, "is thinking");
	}
	return (NULL);
}

void	end_philos(t_vars *vars, t_philo *philos)
{
	int i;

	i = 0;
	while (i < vars->philo_num)
		pthread_join(philos[i++].thread_id, NULL);
	i = 0;
	while (i < vars->fork_num)
		pthread_mutex_destroy(&(vars->forks[i++]));
	pthread_mutex_destroy(&(vars->log));
}

void	death_checker(t_vars *vars, t_philo *philos)
{
	int	i;

	while (!vars->all_eaten)
	{
		i = 0;
		while (i < vars->philo_num && !vars->death_count)
		{
			pthread_mutex_lock(&(vars->meal_check));
			if (time_diff(philos[i].last_meal_ts, timestamp()) > vars->time_to_die)
			{
				philo_print(vars, i, "died");
				vars->death_count++;
			}
			pthread_mutex_unlock(&(vars->meal_check));
			usleep(100);
			i++;
		}
		if (vars->death_count)
			break ;
		i = 0;
		while (vars->must_eat_count != -1 && i < vars->philo_num && philos[i].eat_count >= vars->must_eat_count)
			i++;
		if (i == vars->philo_num)
			vars->all_eaten = 1;
	}
}

t_bool start_philos(t_vars *vars)
{
	int i;
	t_philo *philos;

	i = 0;
	philos = vars->philos;
	vars->init_timestamp = timestamp();
	while (i < vars->philo_num)
	{
		if (pthread_create(&(philos[i].thread_id), NULL, philo_routine, &(philos[i])))
			return (FALSE);
		philos[i].last_meal_ts = timestamp();
		i++;
	}
	death_checker(vars, vars->philos);
	end_philos(vars, philos);
	return (TRUE);
}