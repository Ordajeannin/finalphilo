/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 17:32:22 by ajeannin          #+#    #+#             */
/*   Updated: 2023/06/26 20:32:30 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Parcourt les tableaux de philos et forks,
 * libere la memoire de chaque element,
 * puis libere la memoire des tableaux.
 * Apres reflexion, je n'ai peut etre besoin de free que les tableaux,
 * cette technique ne semble s'appliquer que pour les listes chainees
 * En fonction des conditions de l'appel a cette fonction, peut renvoyer
 * un message d'erreur
*/
int	clean_free(t_philo *philos, t_fork *forks, char *error)
{
	free(philos);
	free(forks);
	if (error != NULL)
		return (msg(error));
	return (0);
}
