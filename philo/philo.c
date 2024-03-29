/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:55:24 by lamici            #+#    #+#             */
/*   Updated: 2024/01/23 11:45:10 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*ft_philo(void *vargp)
{
	t_philo		*philo;
	int			eat_ammount;

	philo = (t_philo *)vargp;
	eat_ammount = philo->info->eat_ammount;
	while (!ft_isdead(philo) && eat_ammount)
	{
		ft_eat(philo);
		if (eat_ammount != -1)
			eat_ammount--;
		if (!eat_ammount)
			break ;
		ft_print("is sleeping\n", philo);
		usleep(1000 * philo->info->sleep_time);
		ft_print("is thinking\n", philo);
	}
	if (!eat_ammount)
	{
		pthread_mutex_lock(philo->actions->eat);
		philo->eat_check = 1;
		pthread_mutex_unlock(philo->actions->eat);
	}
	return (0);
}	

static void	*ft_watcher(void *vargp)
{
	t_philo				*philos;
	static int			i;

	philos = (t_philo *)vargp;
	while (!ft_eat_check(philos, i) && philos->info->eat_ammount)
	{
		i++;
		usleep(100);
		if (i == philos->info->philo_number)
			i = 0;
		pthread_mutex_lock(philos[i].actions->eat);
		if ((ft_timer(philos[i].telapsed, philos[i].actions->clock) > \
			philos[i].info->die_time) && \
				philos[i].telapsed && !(philos[i]).eat_check)
		{
			ft_print("has died\n", &philos[i]);
			pthread_mutex_lock(philos[i].actions->death);
			*philos[i].death = 1;
			pthread_mutex_unlock(philos[i].actions->death);
			pthread_mutex_unlock(philos[i].actions->eat);
			break ;
		}
		pthread_mutex_unlock(philos[i].actions->eat);
	}
	return (NULL);
}

static void	ft_start_threads(t_philo *philo, pthread_mutex_t *mutexes)
{
	pthread_t			observer;

	ft_alt_launch(philo);
	usleep(1000);
	pthread_create(&observer, NULL, ft_watcher, philo);
	pthread_join(observer, NULL);
	ft_kill_philo(philo);
	ft_kill_mutex(philo, mutexes);
}

void	ft_give_info(pthread_t *philos, t_philo *philo, \
			t_info *info, pthread_mutex_t *mutexes)
{
	int					i;
	int					death;
	t_mutex				*actions;

	i = 0;
	death = 0;
	actions = act_init();
	while (i < info->philo_number)
	{
		philo[i].id = i + 1;
		ft_give_forks(mutexes, &philo[i], info);
		philo[i].telapsed = 0;
		philo[i].info = info;
		philo[i].philo = philos[i];
		philo[i].eat_check = 0;
		philo[i].death = &death;
		philo[i].actions = actions;
		i++;
	}
	ft_start_threads(philo, mutexes);
}

int	main(int argc, char **argv)
{
	t_info	*info;
	int		check;

	check = 0;
	if (argc == 6)
		check = 1;
	if (argc < 5 || argc > 6)
		return (printf("Error\n") * 0 + 1);
	else if (!ft_error_deal(4 + check, argv + 1))
	{
		info = ft_init_info(argc, argv);
		ft_create_philo(info);
	}
	else
		return (printf("Error\n") * 0 + 2);
	return (0);
}
