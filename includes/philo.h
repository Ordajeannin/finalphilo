/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 19:02:04 by ajeannin          #+#    #+#             */
/*   Updated: 2023/11/23 18:20:39 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

# define INVALID_INPUT	"the inputs must be 4 or 5 strictly positive integers"
# define ERROR_M_INIT	"mutex can't be initialized, clean exit"
# define ERROR_MALLOC	"malloc does not work as expected"
# define ERROR_PLANNING "can't create philo's threads"
# define ERROR_DEATH	"can't create death's thread"
# define ERROR_WAITING	"can't wait all the threads to end"
# define ERROR_CANT_EAT "don't let enough time to eat and sleep"
# define FREE 			0
# define USED 			1

typedef struct s_fork
{
	int					is_use;
	pthread_mutex_t		lock;
}	t_fork;

typedef struct s_utils
{
	int					nb_of_philos;
	int					t_to_die;
	int					t_to_eat;
	int					t_to_sleep;
	int					meal_max;
	int					end;
	unsigned long int	start_time;
	pthread_mutex_t		console_mutex;
	int					is_dead;
	pthread_mutex_t		m_is_dead;
	pthread_mutex_t		m_stop_death;
	int					stop;
	pthread_t			death_thread;
}	t_utils;

typedef struct s_philo
{
	int					id;
	pthread_t			day;
	unsigned long int	last_meal;
	pthread_mutex_t		m_last_meal;
	int					meal_nb;
	t_fork				*r_fork;
	t_fork				*l_fork;
	int					r_taken;
	int					l_taken;
	t_utils				*utils;
}	t_philo;

char	*parsing(int ac, char **av, t_utils *utils);
char	*philos_birth(t_philo **philos, t_fork **forks, t_utils *utils);
char	*philos_educ(t_philo *philo, t_fork **forks, t_utils *utils, int cur);
char	*philos_day_plan(t_philo **philos, t_utils *utils);
char	*end_of_journey(t_philo **philos, t_utils *utils);
void	*live_their_day(void *arg);
int		clean_free(t_philo *philos, t_fork *forks, char *error);
int		msg(char *str);
int		is_dead(t_philo *philo);
void	stop_threads(t_philo *philo);
void	*check_all_philos_death(void *arg);
void	ft_usleep(long int time_in_ms);
void	write_state(char *str, t_philo *philo);
void	take_fork(char fork_name, t_philo *philo);
void	release_fork(char fork_name, t_philo *philo);
void	release_forks_and_sleep(t_philo *philo);
void	ft_eat_while_alive(t_philo *philo);
long	get_timestamp(void);
int		ft_atoi(const char *str);
int		ft_strlen(const char *s);

#endif
