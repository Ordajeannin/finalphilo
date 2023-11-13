/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 18:50:50 by ajeannin          #+#    #+#             */
/*   Updated: 2023/06/27 20:01:52 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Fonction principale, qui va appeler toutes les autres fonctions clefs.
 * Dans l'ordre :
 * 1) "parsing" va checker si les arguments sont corrects, et init utils
 * 2) "philo_birth" va creer autant de philos + forks que necessaire 
 * 	   et les initialiser/associer une l_fork/r_fork a chacun
 * 3) "philo_day_plan" va creer les threads des philos
 * 4) "end_of_journey" va attendre la fin de l'execution de ces threads
 * 5) Si tout se passe bien, on libere philos et forks, puis return 0
 * 
 * Si erreur, un message specifique a celle ci sera affiche dans la console
*/
int	main(int ac, char **av)
{
	t_utils		utils;
	t_philo		*philos;
	t_fork		*forks;
	char		*error;

	error = NULL;
	error = parsing(ac, av, &utils);
	if (error != NULL)
		return (msg(error));
	error = philos_birth(&philos, &forks, &utils);
	if (error != NULL)
		return (msg(error));
	error = philos_day_plan(&philos, &utils);
	if (error != NULL)
	{	
		stop_threads(&philos[0]);
		return (msg(error));
	}
	error = end_of_journey(&philos, &utils);
//	utils.end++;
	if (error != NULL)
		return (clean_free(philos, forks, error));
	return (clean_free(philos, forks, error));
}
