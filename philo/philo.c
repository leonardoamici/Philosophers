/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:55:24 by lamici            #+#    #+#             */
/*   Updated: 2023/05/02 15:55:09 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*ft_philo(void *vargp)
{
	t_philo		*philo;
	int		eat_ammount;
	
	philo = (t_philo *)vargp;
	eat_ammount = philo->info->eat_ammount;
	while(!*(philo->death) && eat_ammount)
	{
		ft_eat(philo);
		if(eat_ammount != -1)
			eat_ammount--;
		if(!eat_ammount)
			break ;
		ft_print("is sleeping\n", philo);
		usleep(1000 * philo->info->sleep_time);
		ft_print("is thinking\n", philo);
	}
	if(!eat_ammount)
	{
		pthread_mutex_lock(philo->actions->eat);
		philo->eat_check = 1;
		pthread_mutex_unlock(philo->actions->eat);
	}
	return (0);
}	

void	*ft_watcher(void *vargp)
{
	t_philo		*philos;
	int			i;
	int		count;

	i = 0;
	philos = (t_philo *)vargp;
	count = 0;
	while (1)
	{
		i++;
		if (i == philos->info->philo_number)
			i = 0;
		pthread_mutex_lock(philos[i].actions->death);
		if ((ft_timer( philos[i].telapsed, philos[i].actions->clock) > philos[i].info->die_time) && philos[i].telapsed && !(philos[count]).eat_check)
		{
			pthread_mutex_lock(philos[i].actions->print);
			printf("%llu %d has died", ft_timer(philos[i].info->conception, philos[i].actions->clock), philos[i].id);
			*philos[i].death = 1;
			pthread_mutex_unlock(philos[i].actions->print);
			pthread_mutex_unlock(philos[i].actions->death);
			break ;
		}
		pthread_mutex_unlock(philos[i].actions->death);
		pthread_mutex_lock(philos[i].actions->eat);
		if((philos[count]).eat_check)
			count++;
		if(count == philos[i].info->philo_number)
			break;
		pthread_mutex_unlock(philos[i].actions->eat);
	}
	return (NULL);
}

void	ft_give_info(pthread_t *philos, t_philo *philo, t_info *info, pthread_mutex_t *mutexes)
{
	int		i;
	int		death;
	t_mutex				*actions;
	pthread_t			observer;
	
	i = 0;
	death = 0;
	actions = act_init();
	while (i < info->philo_number)
	{
		ft_give_forks(mutexes, &philo[i], i, info);
		philo[i].telapsed = 0;
		philo[i].id = i + 1;
		philo[i].info = info;
		philo[i].philo = philos[i];
		philo[i].eat_check = 0;
		philo[i].death = &death;
		philo[i].actions = actions;
		i++;
	}
	ft_alt_launch(philo);
	usleep(1000);
	pthread_create(&observer, NULL, ft_watcher, philo);
	pthread_join(observer, NULL);
	ft_kill_philo(philo);
	ft_kill_mutex(philo, mutexes);
}

void	ft_create_philo(t_info *info)
{
	t_philo      		*philo;
	pthread_mutex_t		*mutexes;
	pthread_t			*philos;
	int					i;

	i = 0;
	philo = malloc(sizeof(t_philo) * info->philo_number);
	mutexes = malloc(sizeof(pthread_mutex_t) * info->philo_number);
	while (i < info->philo_number)
		pthread_mutex_init(&mutexes[i++], NULL);
	philos = malloc(sizeof(pthread_t) * info->philo_number);
	if(info->philo_number > 0)
		ft_give_info(philos, philo, info, mutexes);
	free(philos);
	free(mutexes);
}

int main(int argc, char **argv)
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