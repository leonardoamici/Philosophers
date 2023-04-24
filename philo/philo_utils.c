/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 09:26:56 by lamici            #+#    #+#             */
/*   Updated: 2023/04/24 10:16:54 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_detach(t_philo *philos)
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
}

void	ft_usleep(int time)
{
	int		i;

	i = 0;
	while(i < time)
		i++;
}

void	ft_print(char *str, t_philo *philo)
{
	if (!*(philo->death))
	{
		pthread_mutex_lock(philo->actions->print);
		printf("%lld %d %s", ft_clock(philo->info->conception, philo->actions->death), philo->id, str);
		pthread_mutex_unlock(philo->actions->print);
	}
}

long long unsigned		ft_clock(long long unsigned otime, pthread_mutex_t *clock)
{
	pthread_mutex_lock(clock);
	struct timeval	time;
	long long unsigned		x;

	gettimeofday(&time, NULL);
	x = time.tv_usec - otime;
	pthread_mutex_unlock(clock);
	return(x);
}

void	ft_alt_launch(t_philo *philos)
{
	int		i;
	long long int	time;

	i = 0;
	time = ft_clock(0, philos->actions->clock);
	while(i < philo->info->philo_number)
	{
		philo[i].info->conception = time;
		pthread_create(&philo[i].philo, NULL, ft_philos, &philo[i]);
		i = i + 2;
	}
	i = 1;
	while(i < philo->info->philo_number)
	{
		philo[i].info->conception = time;
		pthread_create(&philo[i].philo, NULL, ft_philos, &philo[i]);
		i = i + 2;
	}
}