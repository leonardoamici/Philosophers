/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:55:24 by lamici            #+#    #+#             */
/*   Updated: 2023/04/17 09:22:09 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int		g_check = 0;

long long unsigned		ft_clock(long long unsigned otime)
{
	struct timeval	time;
	long long unsigned		x;

	gettimeofday(&time, NULL);
	x = time.tv_usec - otime;
	return(x);
}

void	ft_print(char *str, int time, int id, pthread_mutex_t *print)
{
	pthread_mutex_lock(print);
	printf("%d %d %s", time, id, str);
	pthread_mutex_unlock(print);
}

void      *prova(void *vargp)
{
	t_philo   *philo;
	int		eat_time;
	long long unsigned		last_eat;

	philo = (t_philo *)vargp;
	eat_time = philo->info->eat_ammount;
	last_eat = philo->info->conception;
	while(eat_time && !g_check)
	{
		ft_print("is thinking\n", ft_clock(philo->info->conception), philo->id, philo->info->print);
		pthread_mutex_lock(philo->right);
		ft_print("has taken right fork\n", ft_clock(philo->info->conception), philo->id, philo->info->print);
		pthread_mutex_lock(philo->left);
		ft_print("has taken left fork\n", ft_clock(philo->info->conception), philo->id, philo->info->print);
		pthread_mutex_lock(philo->info->clock);
		philo->telapsed = ft_clock(0) - last_eat;
		last_eat = ft_clock(0);
		pthread_mutex_unlock(philo->info->clock);
		ft_print("is eating\n", ft_clock(philo->info->conception), philo->id, philo->info->print);
		usleep(philo->info->eat_time);
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		ft_print("is sleeping\n", ft_clock(philo->info->conception), philo->id, philo->info->print);
		usleep(philo->info->sleep_time);
		eat_time--;
	}
	return(0);
}

void	ft_detach(t_philo *philos)
{
	int		i;

	i = 0;
	while(i < philos->info->philo_number)
	{
		pthread_detach(philos[i].philo);
		i++;
	}
}

void	*ft_watcher(void *vargp)
{
	t_philo		*philos;
	int			i;

	i = 0;
	philos = (t_philo *)vargp;
	while (1)
	{
		i++;
		if (i == philos->info->philo_number)
			i = 0;
		pthread_mutex_lock(philos->info->clock);
		if (philos[i].telapsed > philos[i].info->die_time)
		{	
			printf("telapsed was %d, ttd was %d\n", philos[i].telapsed, philos->info->die_time);
			printf("philo %d is dead, he was killed\n", philos[i].id);
			g_check = 1;
			ft_detach(philos);
			break ;
		}
		pthread_mutex_unlock(philos->info->clock);
	}
	return (NULL);
}

t_info	*ft_init_info(int argc, char **argv)
{
	t_info	*info;
	struct timeval	startingtime;

	info = malloc(sizeof(t_info) * 1);
	info->clock = malloc(sizeof(pthread_mutex_t) * 1);
	info->print = malloc(sizeof(pthread_mutex_t) * 1);
	info->philo_number = atoi(argv[1]);
	info->die_time = atoi(argv[2]);
	info->eat_time = atoi(argv[3]);
	info->sleep_time = atoi(argv[4]);
	gettimeofday(&startingtime, NULL);
	info->conception = startingtime.tv_usec;
	pthread_mutex_init(info->clock, NULL);
	pthread_mutex_init(info->print, NULL);
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
	}
	else
	{
		philo->left = &mutexes[i];
		philo->right = &mutexes[i + 1];
	}
}
void	ft_create_philo(t_info *info)
{
	t_philo      		*philo;
	pthread_mutex_t		*mutexes;
	pthread_t			*philos;
	pthread_t			observer;
	int	i;

	i = 0;
	philo = malloc(sizeof(t_philo) * info->philo_number);
	mutexes = malloc(sizeof(pthread_mutex_t) * info->philo_number);
	while (i < info->philo_number)
	{
		pthread_mutex_init(&mutexes[i], NULL);
		i++;
	}
	i = 0;
	philos = malloc(sizeof(pthread_t) * info->philo_number);
	while (i < info->philo_number)
	{
		ft_launch_philo(mutexes, &philo[i], i, info);
		philo[i].telapsed = 0;
		philo[i].id = i;
		philo[i].info = info;
		philo[i].philo = philos[i];
		pthread_create(&philo[i].philo, NULL, prova, &philo[i]);
		i = i + 2;
	}
	i = 1;
	usleep(10);
	while (i < info->philo_number)
	{
		ft_launch_philo(mutexes, &philo[i], i, info);
		philo[i].telapsed = 0;
		philo[i].id = i;
		philo[i].info = info;
		philo[i].philo = philos[i];
		pthread_create(&philo[i].philo, NULL, prova, &philo[i]);
		i = i + 2;
	}
	pthread_create(&observer, NULL, ft_watcher, philo);
	i = 0;
	while(!g_check)
		usleep(1);
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