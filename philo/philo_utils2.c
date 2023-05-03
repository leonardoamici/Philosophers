/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 15:04:12 by lamici            #+#    #+#             */
/*   Updated: 2023/05/03 16:26:59 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_alt_launch(t_philo *philos)
{
	int		i;
	int		j;

	i = 0;
	j = philos->info->philo_number % 2;
	if (philos->info->philo_number == 1)
	{
		pthread_create(&philos[i].philo, NULL, ft_lonely_boy, &philos[i]);
	}
	else
	{
		while (i < philos->info->philo_number && \
			philos->info->philo_number != 1)
		{
			pthread_create(&philos[i].philo, NULL, ft_philo, &philos[i]);
			if (i % 2 == 0 && i + 1 != philos->info->philo_number)
				usleep(1000);
			i++;
		}
	}
}

void	ft_give_forks(pthread_mutex_t *mutexes, t_philo *philo, t_info *info)
{
	int	i;

	i = philo->id - 1;
	if (i == (info->philo_number - 1))
	{
		philo->left = &mutexes[i];
		philo->right = &mutexes[0];
	}
	else
	{
		philo->left = &mutexes[i];
		philo->right = &mutexes[i + 1];
	}
}

t_info	*ft_init_info(int argc, char **argv)
{
	t_info	*info;

	info = malloc(sizeof(t_info) * 1);
	info->philo_number = atoi(argv[1]);
	info->die_time = atoi(argv[2]);
	info->eat_time = atoi(argv[3]);
	info->sleep_time = atoi(argv[4]);
	info->conception = ft_clock();
	if (argc == 6)
		info->eat_ammount = atoi(argv[5]);
	else
		info->eat_ammount = -1;
	return (info);
}

t_mutex	*act_init(void)
{
	t_mutex				*actions;

	actions = malloc(sizeof(t_mutex));
	actions->death = malloc(sizeof(pthread_mutex_t));
	actions->clock = malloc(sizeof(pthread_mutex_t));
	actions->print = malloc(sizeof(pthread_mutex_t));
	actions->eat = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(actions->death, NULL);
	pthread_mutex_init(actions->clock, NULL);
	pthread_mutex_init(actions->print, NULL);
	pthread_mutex_init(actions->eat, NULL);
	return (actions);
}
