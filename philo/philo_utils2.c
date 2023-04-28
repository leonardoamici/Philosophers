/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:04:12 by lamici            #+#    #+#             */
/*   Updated: 2023/04/28 15:05:29 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_alt_launch(t_philo *philos)
{
	int		i;

	i = 0;
	while(i < philos->info->philo_number)
	{
		if(philos[i].id % 2 == 0)
			usleep(1000);
		pthread_create(&philos[i].philo, NULL, ft_philo, &philos[i]);
		i++;
	}
}