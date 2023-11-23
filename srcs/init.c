/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 17:40:54 by ajeannin          #+#    #+#             */
/*   Updated: 2023/11/23 17:15:25 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	are_correct(const char *str)
{
	int	i;
	int	res;

	i = 0;
	res = 0;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		i++;
	}
	res = ft_atoi(str);
	return (res);
}

/*
 * Parsing des arguments en entree.
 * Va verifier la validite des arguments et les assigner aux var de utils.
 * Si >= 1 argument est invalide, message d'erreur.
 * Dans l'ordre :
 * 1) verification du nb d'arg + validite
 * 2) assignation des valeurs
 * 3) initialisation des mutex
*/
char	*parsing(int ac, char **av, t_utils *utils)
{
	if (ac != 5 && ac != 6)
		return (INVALID_INPUT);
	utils->nb_of_philos = are_correct(av[1]);
	utils->t_to_die = are_correct(av[2]);
	utils->t_to_eat = are_correct(av[3]);
	utils->t_to_sleep = are_correct(av[4]);
	if (ac == 6)
		utils->meal_max = are_correct(av[5]);
	else
		utils->meal_max = 1;
	utils->is_dead = 0;
	if (utils->nb_of_philos <= 0
		|| utils->t_to_die <= 0
		|| utils->t_to_eat <= 0
		|| utils->t_to_sleep <= 0
		|| utils->meal_max <= 0)
		return (INVALID_INPUT);
	if (ac != 6)
		utils->meal_max = -1;
	if (pthread_mutex_init(&(utils->console_mutex), NULL) != 0
		|| pthread_mutex_init(&(utils->m_is_dead), NULL) != 0)
		return (ERROR_M_INIT);
	utils->end = 0;
	utils->stop = 0;
	return (NULL);
}
