/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:55:24 by lamici            #+#    #+#             */
/*   Updated: 2023/04/24 15:31:36 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*ft_philo_even(void *vargp)
{
	t_philo		*philo;
	struct timeval	cock;
	int		eat_ammount;
	
	philo = (t_philo *)vargp;
	eat_ammount = philo->info->eat_ammount;
	while(eat_ammount && !*(philo->death))
	{
		pthread_mutex_lock(philo->left);
		ft_print("has taken left fork\n", philo);
		pthread_mutex_lock(philo->right);
		ft_print("has taken right fork\n", philo);
		pthread_mutex_lock(philo->actions->death);
		gettimeofday(&cock, NULL);
		philo->telapsed = cock.tv_usec;
		pthread_mutex_unlock(philo->actions->death);
		ft_print("is eating\n", philo);
		usleep(philo->info->eat_time);
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		ft_print("is sleeping\n", philo);
		usleep(philo->info->sleep_time);
		ft_print("is thinking\n", philo);
		eat_ammount--;
	}
	if(!eat_ammount)
	{
		pthread_mutex_lock(philo->actions->eat);
		*philo->eat_check = 1;
		pthread_mutex_unlock(philo->actions->eat);
		ft_print("----------------------------------------------------------------------------\n", philo);
	}
	return (0);
}	

void	*ft_philo_odd(void *vargp)
{
	t_philo		*philo;
	int		eat_ammount;

	philo = (t_philo *)vargp;
	eat_ammount = philo->info->eat_ammount;
	while(eat_ammount && !*(philo->death))
	{
		pthread_mutex_lock(philo->right);
		ft_print("has taken right fork\n", philo);
		pthread_mutex_lock(philo->left);
		ft_print("has taken left fork\n", philo);
		ft_print("is eating\n", philo);
		usleep(philo->info->eat_time * 1000);
		pthread_mutex_lock(philo->actions->death);
		philo->telapsed = ft_clock(0, philo->actions->clock);
		pthread_mutex_unlock(philo->actions->death);
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		ft_print("is sleeping\n", philo);
		usleep(philo->info->sleep_time * 1000);
		ft_print("is thinking\n", philo);
		eat_ammount--;
	}
	if(!eat_ammount)
	{
		pthread_mutex_lock(philo->actions->eat);
		*philo->eat_check = 1;
		pthread_mutex_unlock(philo->actions->eat);
		ft_print("----------------------------------------------------------------------------\n", philo);
	}
	return (0);
}

void	*ft_watcher(void *vargp)
{
	t_philo		*philos;
	struct timeval	time;
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
		gettimeofday(&time, NULL);
		if ((time.tv_usec - philos[i].telapsed > philos[i].info->die_time) && philos[i].telapsed)
		{
			pthread_mutex_lock(philos[i].actions->print);
			printf("telapsed was %llu, ttd was %llu\n", ft_clock(philos[i].telapsed, philos[i].actions->clock), philos->info->die_time);
			printf("philo %d is dead", philos[i].id);
			ft_detach(philos);
			break ;
		}
		pthread_mutex_unlock(philos[i].actions->death);
		pthread_mutex_lock(philos[i].actions->eat);
		if(*(philos[count]).eat_check)
			count++;
		if(count == philos[i].info->philo_number)
			break;
		pthread_mutex_unlock(philos[i].actions->eat);
	}
	return (NULL);
}

t_info	*ft_init_info(int argc, char **argv)
{
	t_info	*info;
//	struct timeval	time;

	info = malloc(sizeof(t_info) * 1);
	info->philo_number = atoi(argv[1]);
	info->die_time = atoi(argv[2]);
	info->eat_time = atoi(argv[3]);
	info->sleep_time = atoi(argv[4]);
//	gettimeofday(&time, NULL);
//	info->conception = time.tv_usec;
	if (argc == 6)
		info->eat_ammount = atoi(argv[5]);
	else
		info->eat_ammount = -1;
	return (info);
}

void	 ft_launch_philo(pthread_mutex_t *mutexes, t_philo *philo, int i, t_info *info)
{
	if (i == (info->philo_number - 1))
	{
		philo->left = &mutexes[i];
		philo->right = &mutexes[0];
		//printf("gave fork %d and 0\n", i);
	}
	else
	{
		philo->left = &mutexes[i];
		philo->right = &mutexes[i + 1];
		//printf("gave fork %d and %d\n", i, (i + 1));
	}
}
t_mutex		*act_init(void)
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

void	ft_create_philo(t_info *info)
{
	t_philo      		*philo;
	pthread_mutex_t		*mutexes;
	pthread_t			*philos;
	pthread_t			observer;
	t_mutex				*actions;
	int					*eat_time;
	int					*deaths;
	int					i;

	i = 0;
	actions = act_init();
	philo = malloc(sizeof(t_philo) * info->philo_number);
	mutexes = malloc(sizeof(pthread_mutex_t) * info->philo_number);
	eat_time = val_set(info->philo_number);
	deaths = val_set(info->philo_number);
	while (i < info->philo_number)
		pthread_mutex_init(&mutexes[i++], NULL);
	i = 0;
	philos = malloc(sizeof(pthread_t) * info->philo_number);
	while (i < info->philo_number)
	{
		ft_launch_philo(mutexes, &philo[i], i, info);
		philo[i].telapsed = 0;
		philo[i].id = i;
		philo[i].info = info;
		philo[i].philo = philos[i];
		philo[i].eat_check = &eat_time[i];
		philo[i].death = &deaths[i];
		philo[i].actions = actions;
		i++;
	}
	//fclose(stdout);
	i = 0;
	ft_alt_launch(philo);
	pthread_create(&observer, NULL, ft_watcher, philo);
	pthread_join(observer, NULL);
}

int main(int argc, char **argv)
{
	t_info	*info;
	int		check;

	check = 0;
	if (argc == 6)
		check = 1;
	if (argc < 5 || argc > 6)
	{
		printf("Error\n");
		return (1);
	}
	else if (!ft_error_deal(4 + check, argv + 1))
	{
		info = ft_init_info(argc, argv);
		ft_create_philo(info);
	}
	else
	{
		printf("Error\n");
		return (1);
	}
	return (0);
}