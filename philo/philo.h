#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

struct	s_vars;

typedef enum e_bool
{
	TRUE = 1,
	FALSE = 0
}	t_bool;

typedef struct s_philo
{
	unsigned int	id;
	int				left_fork;
	int				right_fork;
	int				eat_count;
	long			last_meal_ts;
	pthread_t		thread_id;
	struct s_vars	*vars;
}	t_philo;

typedef struct s_vars
{
	int				philo_num;
	int				fork_num;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	long			init_timestamp;
	int				death_count;
	int				all_eaten;
	t_philo			philos[250];
	pthread_mutex_t	forks[250];
	pthread_mutex_t	eating;
	pthread_mutex_t	log;
	pthread_mutex_t	var_change;
}	t_vars;

t_bool		start_philos(t_vars *vars);
int			ft_atoi(const char *str);
void		philo_print(t_vars *vars, unsigned int id, char *message);
void		delay(long duration, t_vars *vars);
long		timestamp(void);
void		acknowledge_death(t_vars *vars, int id);
int			err_print(char *message);

#endif