/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/26 16:44:20 by ajeannin          #+#    #+#             */
/*   Updated: 2023/11/23 17:41:27 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Permet de prendre une fourchette specifiee
 * Dans l'ordre :
 * 1) Verifie si le philosophe est en vie
 * 2) mise a jour des pointeurs
 * 3) lock, verifie la disponibilite de la fourchette
 * 4) si dispo, mise a jour des pointeurs puis unlock
*/
void	take_fork(char fork_name, t_philo *philo)
{
	int		*taken;
	t_fork	*fork;

	if (!is_dead(philo))
	{
		taken = &(philo->r_taken);
		fork = philo->r_fork;
		if (fork_name == 'l')
		{
			taken = &(philo->l_taken);
			fork = philo->l_fork;
		}
		pthread_mutex_lock(&(fork->lock));
		if (!(*taken) && !fork->is_use)
		{
			*taken = USED;
			fork->is_use = USED;
			pthread_mutex_unlock(&(fork->lock));
			write_state("has taken a fork", philo);
		}
		else
			pthread_mutex_unlock(&(fork->lock));
	}
}

/*
 * Permet de relacher une fourchette specifiee
 * Dans l'ordre :
 * mise a jour des pointeurs
 * lock, "je suis dispo", unlock
*/
void	release_fork(char fork_name, t_philo *philo)
{
	int		*taken;
	t_fork	*fork;

	taken = &(philo->r_taken);
	fork = philo->r_fork;
	if (fork_name == 'l')
	{
		taken = &(philo->l_taken);
		fork = philo->l_fork;
	}
	pthread_mutex_lock(&(fork->lock));
	*taken = FREE;
	fork->is_use = FREE;
	pthread_mutex_unlock(&(fork->lock));
}

/*
 * Release les deux fourchettes apres avoir mange
 * Ecrit l'etat du philo dans la console, puis sleep t_to_sleep
 * (verifie constamment si un philo est mort pendant t_to_sleep,
 * pour arreter la simulation des qu'un philo meurt sans attendre 
 * que les autres se reveillent)
 * Des le reveil, "thinking" jusqu'au prochain repas
*/
void	release_forks_and_sleep(t_philo *philo)
{
	int	sleep;

	sleep = philo->utils->t_to_sleep;
	release_fork('r', philo);
	release_fork('l', philo);
	if (philo->meal_nb != philo->utils->meal_max)
	{
		write_state("is sleeping", philo);
		while (sleep-- > 0)
		{
			pthread_mutex_lock(&(philo->utils->m_is_dead));
			if (philo->utils->is_dead == 0)
			{
				pthread_mutex_unlock(&(philo->utils->m_is_dead));
				ft_usleep(1);
			}
			else
			{
				pthread_mutex_unlock(&(philo->utils->m_is_dead));
				return ;
			}
		}
		write_state("is thinking", philo);
	}
}
