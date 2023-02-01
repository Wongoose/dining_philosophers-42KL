#include "philo.h"

t_bool	init_mutex(t_vars *vars)
{
	int i;

	i = 0;
	while (i < vars->fork_num)
	{
		if (pthread_mutex_init(&(vars->forks[i]), NULL))
			return (FALSE);
		i++;
	}
	if (pthread_mutex_init(&(vars->log), NULL))
		return (FALSE);
	if (pthread_mutex_init(&(vars->meal_check), NULL))
		return (FALSE);
	return (TRUE);
}

void	init_philosophers(t_vars *vars)
{
	int i;

	i = 0;
	while (i < vars->philo_num)
	{
		vars->philos[i].id = i;
		vars->philos[i].eat_count = 0;
		vars->philos[i].left_fork = i;
		vars->philos[i].right_fork = (vars->philo_num + i - 1) % vars->philo_num;
		vars->philos[i].last_meal_ts = 0;
		vars->philos[i].vars = vars;
		i++;
	}
}

t_bool	init_vars(t_vars *vars, int argc, char **argv)
{
	vars->philo_num = ft_atoi(argv[1]);
	vars->fork_num = ft_atoi(argv[1]);
	vars->time_to_die = ft_atoi(argv[2]);
	vars->time_to_eat = ft_atoi(argv[3]);
	vars->time_to_sleep = ft_atoi(argv[4]);
	vars->all_eaten = 0;
	vars->death_count = 0;
	if (vars->philo_num < 2 || vars->time_to_die < 0 || vars->time_to_eat < 0
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

int main(int argc, char **argv)
{
	t_vars vars;

	if (argc != 5 && argc != 6)
		return (err_print("Invalid number of arguments!"));
	if(init_vars(&vars, argc, argv) == FALSE)
		return (err_print("Invalid argument values!"));
	if(init_mutex(&vars) == FALSE)
		return (err_print("Failed to initialize mutexes!"));
	init_philosophers(&vars);
	if(start_philos(&vars) == FALSE)
		return (err_print("Failed to initialize threads!"));
	return (0);
}