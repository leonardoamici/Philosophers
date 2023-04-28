/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 09:26:56 by lamici            #+#    #+#             */
/*   Updated: 2023/04/28 15:05:37 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*void	ft_detach(t_philo *philos)
{
	int		i;
	static int	check;

	i = 0;
	check = 0;
	while(i < philos->info->philo_number && !check)
	{
		pthread_detach(philos[i].philo);
		i++;
	}
	check++;
}*/

int		ft_isdead(t_philo *philo)
{
	int		i;

	i = 0;
	pthread_mutex_lock(philo->actions->death);
	if(*(philo->death))
		i = 1;
	pthread_mutex_unlock(philo->actions->death);
	return (i);
}

void	ft_print(char *str, t_philo *philo)
{
	pthread_mutex_lock(philo->actions->print);
	if(!*(philo->death))
		printf("%llu %d %s", ft_timer(philo->info->conception, philo->actions->clock), philo->id, str);
	pthread_mutex_unlock(philo->actions->print);
}

long long unsigned		ft_timer(long long unsigned otime, pthread_mutex_t *clock)
{
	pthread_mutex_lock(clock);
	struct timeval	time;
	long long unsigned		x;

	gettimeofday(&time, NULL);
	x = ((time.tv_usec / 1000) + (time.tv_sec * 1000)) - otime;
	pthread_mutex_unlock(clock);
	return(x);
}

long long unsigned		ft_clock()
{
	struct timeval	time;
	long long unsigned		x;

	gettimeofday(&time, NULL);
	x = (time.tv_usec / 1000) + (time.tv_sec * 1000);
	
	return(x);
}