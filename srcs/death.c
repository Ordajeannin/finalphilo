/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 17:46:20 by ajeannin          #+#    #+#             */
/*   Updated: 2023/06/27 19:51:30 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Recupere la valeur de is_dead, protege par un mutex.
 * lock, recupere, unlock le mutex pour eviter le data race
 * retourne 1 si mort, 0 si vivant
*/
int	is_dead(t_philo *philo)
{
	int	alive;

	pthread_mutex_lock(&(philo->utils->m_is_dead));
	alive = philo->utils->is_dead;
	pthread_mutex_unlock(&(philo->utils->m_is_dead));
	return (alive);
}

/*
 * Arrete les threads en mettant a jour is_dead a 1
 * (protege son acces avant de modifier sa valeur)
*/
void	stop_threads(t_philo *philo)
{
	pthread_mutex_lock(&(philo->utils->m_is_dead));
	philo->utils->is_dead = 1;
	pthread_mutex_unlock(&(philo->utils->m_is_dead));
	return ;
}

/*
 * Verifie si un philo est mort en comparant le tps ecoule depuis son dernier
 * repas avec le [t_to_die]
 * -> protege puis recupere last_meal
 * -> cur_time - last_meal
 * -> si > a [t_to_die], le philo est mort -> protege puis set is_dead a 1
 * into -> affiche un message dans le terminal
 * si vivant, renvoie 0, puis prochain philo
*/
static int	check_death_for_each_philo(t_philo *philo, long cur_time)
{
	int	dead;
	int	last_meal;

	dead = 0;
	pthread_mutex_lock(&(philo->m_last_meal));
	last_meal = cur_time - philo->last_meal;
	pthread_mutex_unlock(&(philo->m_last_meal));
	if (last_meal > philo->utils->t_to_die)
	{
		pthread_mutex_lock(&(philo->utils->console_mutex));
		pthread_mutex_lock(&(philo->utils->m_is_dead));
		philo->utils->is_dead = 1;
		pthread_mutex_unlock(&(philo->utils->m_is_dead));
		printf("%09ld %d died\n", cur_time, philo->id + 1);
		pthread_mutex_unlock(&(philo->utils->console_mutex));
		dead = 1;
	}
	return (dead);
}

/*
 * Boucle a l'infini sur tous les philosophes pour checker leurs pouls
 * Prend en parametre la liste des philos, qu'il faut reatribuer
 * (puis recuperer utils grace au premier philo)
 * cur_time est update a chaque iteration -> tps ecoule depuis le start
 * Si un mort est detecte, ou si la simulation est finie, fin de la fonction
 * Sinon, petite attente et on y retourne
 * 
 * Tourne en arriere plan jusqu a ce qu une mort soit detectee
*/
void	*check_all_philos_death(void	*arg)
{
	t_utils	*utils;
	t_philo	**philos;
	long	cur_time;
	int		cur;

	philos = (t_philo **)arg;
	utils = philos[0]->utils;
//	while (utils->end == 0)
	while (1)
	{
		cur = 0;
		cur_time = get_timestamp() - utils->start_time;
		while (cur < utils->nb_of_philos)
		{
			if (check_death_for_each_philo(&(*philos)[cur], cur_time))
				return (NULL);
			cur++;
		}
		ft_usleep(10);
	}
	return (NULL);
}
