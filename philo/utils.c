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
		printf("Philosopher %u ", id + 1);
		printf("%s\n", message);
	}
	pthread_mutex_unlock(&(vars->var_change));
	pthread_mutex_unlock(&(vars->log));
}
