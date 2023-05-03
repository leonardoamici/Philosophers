/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 09:26:56 by lamici            #+#    #+#             */
/*   Updated: 2023/05/03 10:44:31 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isdead(t_philo *philo)
{
	int		i;

	i = 0;
	pthread_mutex_lock(philo->actions->death);
	if (*(philo->death))
		i = 1;
	pthread_mutex_unlock(philo->actions->death);
	return (i);
}

void	ft_print(char *str, t_philo *philo)
{
	pthread_mutex_lock(philo->actions->print);
	if (!ft_isdead(philo))
		printf("%llu %d %s", ft_timer(philo->info->conception, \
		philo->actions->clock), philo->id, str);
	pthread_mutex_unlock(philo->actions->print);
}

long long unsigned	ft_timer(long long unsigned otime, \
							pthread_mutex_t *clock)
{
	struct timeval			time;
	long long unsigned		x;

	pthread_mutex_lock(clock);
	gettimeofday(&time, NULL);
	x = ((time.tv_usec / 1000) + (time.tv_sec * 1000)) - otime;
	pthread_mutex_unlock(clock);
	return (x);
}

long long unsigned	ft_clock(void)
{
	struct timeval			time;
	long long unsigned		x;

	gettimeofday(&time, NULL);
	x = (time.tv_usec / 1000) + (time.tv_sec * 1000);
	return (x);
}

void	ft_create_philo(t_info *info)
{
	t_philo				*philo;
	pthread_mutex_t		*mutexes;
	pthread_t			*philos;
	int					i;

	i = 0;
	philo = malloc(sizeof(t_philo) * info->philo_number);
	mutexes = malloc(sizeof(pthread_mutex_t) * info->philo_number);
	while (i < info->philo_number)
		pthread_mutex_init(&mutexes[i++], NULL);
	philos = malloc(sizeof(pthread_t) * info->philo_number);
	if (info->philo_number > 0)
		ft_give_info(philos, philo, info, mutexes);
	free(philos);
	free(mutexes);
}
