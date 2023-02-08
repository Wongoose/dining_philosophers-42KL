#include "philo.h"

// Eating is quite complicated
// REMEMBER: Forks are MUTEXES (only can be held by 1 philo)
// if the fork is being used,
// pthread_lock() will wait until the fork is available again
// updates variables
// then unlock the mutexes after eating (return the forks to the table)
t_bool	philo_eats(t_philo *philo)
{
	t_vars	*vars;

	vars = philo->vars;
	pthread_mutex_lock(&(vars->forks[philo->left_fork]));
	philo_print(vars, philo->id, "has taken a fork");
	if (philo->right_fork == -1)
		return (FALSE);
	pthread_mutex_lock(&(vars->forks[philo->right_fork]));
	philo_print(vars, philo->id, "has taken a fork");
	pthread_mutex_lock(&(philo->eating));
	philo_print(vars, philo->id, "is eating");
	philo->last_meal_ts = timestamp();
	(philo->eat_count)++;
	pthread_mutex_unlock(&(philo->eating));
	delay(vars->time_to_eat, vars);
	pthread_mutex_unlock(&(vars->forks[philo->left_fork]));
	pthread_mutex_unlock(&(vars->forks[philo->right_fork]));
	return (TRUE);
}

// Here you see their life, their routine (eat, sleep, think, die maybe...)
// philo->id % 2 basically prevents all the philo from snatching the forks
void	*routine(void *data)
{
	t_philo	*philo;
	t_vars	*vars;

	philo = (t_philo *)data;
	vars = philo->vars;
	if (philo->id % 2)
		usleep(15000);
	pthread_mutex_lock(&(vars->var_change));
	while (!vars->death_count)
	{
		if (vars->all_eaten)
			break ;
		pthread_mutex_unlock(&(vars->var_change));
		if (philo_eats(philo) == FALSE)
			break ;
		philo_print(vars, philo->id, "is sleeping");
		delay(vars->time_to_sleep, vars);
		philo_print(vars, philo->id, "is thinking");
		pthread_mutex_lock(&(vars->var_change));
	}
	pthread_mutex_unlock(&(vars->var_change));
	return (NULL);
}

// This loop runs simultaneously, this guys is like the death witch
// gonna keep checking which philo is dead (according to inputs earlier)
// if there is 1 or more death, break out of death_checker and end the program
void	death_checker(t_vars *vars, t_philo *philos)
{
	int	i;

	while (!vars->all_eaten)
	{
		i = 0;
		while (i < vars->philo_num && !vars->death_count)
		{
			pthread_mutex_lock(&(philos[i].eating));
			if (timestamp() - philos[i].last_meal_ts > vars->time_to_die)
				acknowledge_death(vars, i);
			usleep(100);
			pthread_mutex_unlock(&(philos[i].eating));
			i++;
		}
		if (vars->death_count)
			break ;
		if (check_all_ate(vars, philos) == TRUE)
		{
			pthread_mutex_lock(&(vars->var_change));
			vars->all_eaten = 1;
			pthread_mutex_unlock(&(vars->var_change));
		}
	}
}

// End the program here.
// Clear mutexes, no free() used
void	end_philos(t_vars *vars, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < vars->philo_num)
		pthread_join(philos[i++].thread_id, NULL);
	i = 0;
	while (i < vars->philo_num)
		pthread_mutex_destroy(&(philos[i++].eating));
	i = 0;
	while (i < vars->fork_num)
		pthread_mutex_destroy(&(vars->forks[i++]));
	pthread_mutex_destroy(&(vars->log));
	pthread_mutex_destroy(&(vars->var_change));
}

// The TRUE start of the "game"
// After God pictured the philos, now he pthread_created them
// Each thread has their own lives, running at the same time
// death_checker() also runs in the main thread/program
// so now we look into the lives of each philosopher
t_bool	start_philos(t_vars *vars)
{
	int		i;
	t_philo	*philos;

	i = 0;
	philos = vars->philos;
	vars->init_timestamp = timestamp();
	while (i < vars->philo_num)
	{
		if (pthread_create(&(philos[i].thread_id), NULL, routine, &(philos[i])))
			return (FALSE);
		pthread_mutex_lock(&(philos[i].eating));
		philos[i].last_meal_ts = timestamp();
		pthread_mutex_unlock(&(philos[i].eating));
		i++;
	}
	death_checker(vars, vars->philos);
	end_philos(vars, philos);
	return (TRUE);
}
