/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 18:26:17 by ajeannin          #+#    #+#             */
/*   Updated: 2023/11/23 18:40:03 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Creation des philos + forks necessaire.
 * Dans l'ordre:
 * 1) allocation de memoire pour le tableau de philosophes
 * 2) allocation de memoire pour le tableau de fourchettes
 * 3) initialisation de chaque philo avec "philos_educ"
*/
char	*philos_birth(t_philo **philos, t_fork **forks, t_utils *utils)
{
	int		cur;
	char	*error;

	cur = -1;
	error = NULL;
	*philos = malloc(sizeof(t_philo) * utils->nb_of_philos);
	if (!(*philos))
		return (ERROR_MALLOC);
	*forks = malloc(sizeof(t_fork) * utils->nb_of_philos);
	if (!(*forks))
	{
		free(*philos);
		return (ERROR_MALLOC);
	}
	while (++cur < utils->nb_of_philos)
	{
		error = philos_educ(&(*philos)[cur], forks, utils, cur);
		if (error != NULL)
		{
			free(*philos);
			free(*forks);
			return (error);
		}
	}
	return (NULL);
}

/*
 * "Eduque" un philosophe donne -> initialisation de sa structure.
 * Attribution des fourchettes, et les definies comme etant libres.
 * Le dernier philo aura la premiere fork de la liste a sa gauche.
 * Initialisation des mutex pour la synchronisation + protection.
*/
char	*philos_educ(t_philo *philo, t_fork **forks, t_utils *utils, int cur)
{
	philo->utils = utils;
	philo->id = cur;
	philo->last_meal = 0;
	philo->meal_nb = 0;
	philo->r_fork = &((*forks)[cur]);
	philo->r_taken = FREE;
	philo->l_taken = FREE;
	if (cur == utils->nb_of_philos - 1)
		philo->l_fork = &((*forks)[0]);
	else
		philo->l_fork = &((*forks)[cur + 1]);
	philo->l_fork->is_use = FREE;
	if (pthread_mutex_init(&(philo->m_last_meal), NULL) != 0
		|| pthread_mutex_init(&(philo->l_fork->lock), NULL) != 0
		|| pthread_mutex_init(&(philo->r_fork->lock), NULL) != 0
		|| pthread_mutex_init(&(philo->utils->m_stop_death), NULL) != 0)
		return (ERROR_M_INIT);
	return (NULL);
}

/*
 * On va commencer les choses serieuses.
 * Dans l'ordre:
 * 1) Initialisation du start_time, pour les calculs de duree
 * 2) Creation des threads pour chaque philo
 * 3) Creation d'un thread qui checkera regulierement si un philo est mort
*/
char	*philos_day_plan(t_philo **philos, t_utils *utils)
{
	int	cur;

	cur = 0;
	utils->start_time = get_timestamp();
	while (cur < utils->nb_of_philos)
	{
		if (pthread_create(&((*philos)[cur].day), NULL,
			live_their_day, &((*philos)[cur])))
			return (ERROR_PLANNING);
		cur++;
	}
	if (pthread_create(&(utils->death_thread),
			NULL, check_all_philos_death, philos))
		return (ERROR_DEATH);
	return (NULL);
}

/*
 * Permet d'attendre la fin de l'execution de chaque thread, 
 * grace a "pthread_join"
 * set stop a 1 permet d'arreter death_thread avant qu'une mort ne se declare
 * (si il n'y avait que x repas, par ex)
*/
char	*end_of_journey(t_philo **philos, t_utils *utils)
{
	int		cur;
	char	*error;

	cur = 0;
	error = NULL;
	while (cur < utils->nb_of_philos)
	{
		if (pthread_join((*philos)[cur].day, NULL))
			error = ERROR_WAITING;
		cur++;
	}
	pthread_mutex_lock(&(utils->m_stop_death));
	utils->stop = 1;
	pthread_mutex_unlock(&(utils->m_stop_death));
	if (pthread_join(utils->death_thread, NULL))
		error = ERROR_WAITING;
	return (error);
}

/*
 * Thread pour chaque philosophe
 * "arg" est un pointeur vers la structure du philosophe concerne
 * (ca fonctionne comme ca lorsqu'une fonction est appelee en point d'entree
 * d'un thread, cf "pthread_create" (philos_day_plan)
 * Ainsi, les deux premieres lignes nous permettent de manipuler convenablement
 * notre philo
 * Ensuite, les philos impair vont lancer leurs threads en decale, pour 
 * eviter les data races + conflits
 * Enfin, tant que le philo est en vie (check "is_dead"):
 *  - est ce que le philo a atteint [meal_max] ?
 *  - fourchette g accessible?
 *  - si oui, et la fourchette d?
 *  - si oui, mange -> update meal_nb + last_meal -> dort [t_to_eat]
 *	 -> relache fourchettes -> dort [t_to_sleep]
*/
void	*live_their_day(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 != 0)
		ft_eat_while_alive(philo);
	while (!is_dead(philo))
	{
		if (philo->meal_nb >= philo->utils->meal_max
			&& philo->utils->meal_max > 0)
			break ;
		take_fork('l', philo);
		if (philo->l_taken)
			take_fork('r', philo);
		if (philo->l_taken && philo->r_taken)
		{
			write_state("is eating", philo);
			philo->meal_nb++;
			pthread_mutex_lock(&(philo->m_last_meal));
			philo->last_meal = get_timestamp() - philo->utils->start_time;
			pthread_mutex_unlock(&(philo->m_last_meal));
			ft_eat_while_alive(philo);
			release_forks_and_sleep(philo);
		}
	}
	return (NULL);
}
