/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 18:53:40 by ajeannin          #+#    #+#             */
/*   Updated: 2023/06/26 18:26:43 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * Met en pause l'exécution du programme pendant un certain temps 
 * (donné en millisecondes)
 * La fonction utilise la fonction `get_timestamp` pour obtenir 
 * le temps de départ en millisecondes.
 * Ensuite, elle entre dans une boucle tant que 
 * la différence entre le temps actuel et le temps de départ
 * est inférieure à la durée de pause spécifiée.
 * Pendant chaque itération de la boucle, 
 * utilise la fonction `usleep` pour suspendre l'exécution
 * pendant une période de 100 microsecondes (0,1 milliseconde).
 * Une fois que la durée spécifiée s'est écoulée, 
 * la fonction se termine et le programme reprend son exécution normale.
*/
void	ft_usleep(long int time_in_ms)
{
	long int	start_time;

	start_time = 0;
	start_time = get_timestamp();
	while ((get_timestamp() - start_time) < time_in_ms)
		usleep(100);
}

/*
 * affiche les differentes actions des philos dans le terminal
 * (avec protection)
*/
void	write_state(char *str, t_philo *philo)
{
	long	cur_time;

	cur_time = get_timestamp() - philo->utils->start_time;
	pthread_mutex_lock(&(philo->utils->console_mutex));
	if (!is_dead(philo))
		printf("%09ld %d %s\n", cur_time, philo->id + 1, str);
	pthread_mutex_unlock(&(philo->utils->console_mutex));
}

/*
 * Renvoie le timestamp actuel en millisecondes.
 * La fonction utilise la fonction `gettimeofday` 
 * pour obtenir la structure `timeval`
 * qui contient le temps actuel avec une résolution en microsecondes.
 * Elle multiplie ensuite le nombre de secondes par 1000 
 * pour le convertir en millisecondes,
 * puis ajoute le quotient de la division du nombre de microsecondes par 1000 
 * pour obtenir les millisecondes restantes.
 * Le résultat final est le timestamp actuel en millisecondes.
 * La fonction renvoie ce timestamp.
*/
long	get_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/*
 * fct classique de ma libft
*/
int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 0;
	result = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
		||str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	if (str[i] == '-')
		sign = -1;
	else
		sign = 1;
	if (sign == -1 || str[i] == '+')
		i++;
	result = 0;
	while (str[i] >= '0' && str[i] <= '9')
		result = (result * 10) + (str[i++] - '0');
	return (result * sign);
}

/*
 * fct classique (adapte)
*/
int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
