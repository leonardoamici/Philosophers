/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 09:55:24 by lamici            #+#    #+#             */
/*   Updated: 2023/04/18 14:42:07 by lamici           ###   ########.fr       */
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

void	*prova(void *vargp)
{
	t_philo		*philo;
	int		eat_time;
	long long unsigned		last_eat;

	philo = (t_philo *)vargp;
	eat_time = philo->info->eat_ammount;
	last_eat = ft_clock(0);
	while(eat_time && !philo->death[0])
	{
		ft_print("is thinking\n", ft_clock(philo->info->conception), philo->id, philo->actions->print);
		pthread_mutex_lock(philo->right);
		ft_print("has taken right fork\n", ft_clock(philo->info->conception), philo->id, philo->actions->print);
		pthread_mutex_lock(philo->left);
		ft_print("has taken left fork\n", ft_clock(philo->info->conception), philo->id, philo->actions->print);
		philo->telapsed = ft_clock(0) - last_eat;
		if (philo->telapsed > philo->info->die_time)
		{
			pthread_mutex_lock(philo->actions->death);
			philo->death[philo->id] = 1;
			pthread_mutex_unlock(philo->actions->death);
		}
		ft_print("is eating\n", ft_clock(philo->info->conception), philo->id, philo->actions->print);
		usleep(philo->info->eat_time);
		pthread_mutex_unlock(philo->left);
		pthread_mutex_unlock(philo->right);
		ft_print("is sleeping\n", ft_clock(philo->info->conception), philo->id, philo->actions->print);
		usleep(philo->info->sleep_time);
		eat_time--;
	}
	if(!eat_time)
	{
		pthread_mutex_unlock(philo->actions->eat);
		philo->eat_check[philo->id] = 1;
		pthread_mutex_unlock(philo->actions->eat);
	}
	return(0);
}

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
		if (i == philos->info->philo_number + 1)
			i = 0;
		pthread_mutex_lock(philos[i].actions->death);
		if (philos[i].death[philos[i].id])
		{	
			pthread_mutex_lock(philos[i].actions->print);
			printf("telapsed was %d, ttd was %d\n", philos[i].telapsed, philos->info->die_time);
			printf("philo %d is dead", philos[i].id);
			ft_detach(philos);
			pthread_mutex_unlock(philos[i].actions->print);
			break ;
		}
		pthread_mutex_unlock(philos[i].actions->death);
		pthread_mutex_lock(philos[i].actions->eat);
		//if(philos[i].eat_check[0] == philos[i].info->philo_number)
		//	break ;
		pthread_mutex_unlock(philos[i].actions->eat);
	}
	return (NULL);
}

t_info	*ft_init_info(int argc, char **argv)
{
	t_info	*info;

	info = malloc(sizeof(t_info) * 1);
	info->philo_number = atoi(argv[1]);
	info->die_time = atoi(argv[2]);
	info->eat_time = atoi(argv[3]);
	info->sleep_time = atoi(argv[4]);
	info->conception = ft_clock(0);
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
t_mutex		*act_init(void)
{
	t_mutex				*actions;
	
	actions = malloc(sizeof(t_mutex));
	actions->death = malloc(sizeof(pthread_mutex_t));
	actions->print = malloc(sizeof(pthread_mutex_t));
	actions->eat = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(actions->death, NULL);
	pthread_mutex_init(actions->print, NULL);
	pthread_mutex_init(actions->eat, NULL);
	return (actions);
}

int		*val_set(int num)
{
	int		*temp;
	int		i;

	temp = malloc(sizeof(int) * num);
	i = 0;
	while (i < num)
	{
		temp[i] = 0;
		i++;
	}
	return (temp);
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
		philo[i].eat_check = eat_time;
		philo[i].death = deaths;
		philo[i].actions = actions;
		i++;
	}
	i = -1;
	while (i++ < info->philo_number)
		pthread_create(&philo[i].philo, NULL, prova, &philo[i]);
	pthread_create(&observer, NULL, ft_watcher, philo);
	pthread_join(observer, NULL);
	i = 0;
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