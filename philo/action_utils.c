/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 22:47:43 by lamici            #+#    #+#             */
/*   Updated: 2023/05/03 16:13:45 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_eat(t_philo *philo)
{
	if (philo->right > philo->left)
	{
		//printf("i am philo %d, i went first option\n", philo->id);
		ft_print("has taken left fork\n", philo);
		pthread_mutex_lock(philo->left);
		ft_print("has taken left fork\n", philo);
		pthread_mutex_lock(philo->right);
		ft_print("has taken right fork\n", philo);
	}
	else if (philo->left > philo->right)
	{
		//printf("i am philo %d, i went second option\n", philo->id);
		pthread_mutex_lock(philo->right);
		ft_print("has taken right fork\n", philo);
		pthread_mutex_lock(philo->left);
		ft_print("has taken left fork\n", philo);
	}
	ft_print("is eating\n", philo);
	pthread_mutex_lock(philo->actions->eat);
	philo->telapsed = ft_clock();
	pthread_mutex_unlock(philo->actions->eat);
	usleep(1000 * philo->info->eat_time);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
}

void	*ft_lonely_boy(void *args)
{
	t_philo		*philo;
	int			eat_ammount;

	philo = (t_philo *)args;
	eat_ammount = philo->info->eat_ammount;
	if (eat_ammount != 0)
	{
		ft_print("boy i sure am hungry\n", philo);
		ft_print("looks like i only own a singular fork\n", philo);
		pthread_mutex_lock(philo->actions->death);
		philo->telapsed = ft_clock();
		pthread_mutex_unlock(philo->actions->death);
		usleep(1000 * philo->info->eat_time);
	}
	else
		ft_print("no need to eat im already full of knowledge\n", philo);
	return (NULL);
}

void	ft_kill_philo(t_philo *philos)
{
	int		i;

	i = 0;
	while (i < philos->info->philo_number)
	{
		pthread_join(philos[i].philo, NULL);
		i++;
	}
}

void	ft_kill_mutex(t_philo *philos, pthread_mutex_t *mutexes)
{
	int		i;

	i = 0;
	while (i < philos->info->philo_number)
	{
		pthread_mutex_destroy(&mutexes[i]);
		i++;
	}
	pthread_mutex_destroy(philos->actions->clock);
	pthread_mutex_destroy(philos->actions->print);
	pthread_mutex_destroy(philos->actions->death);
	pthread_mutex_destroy(philos->actions->eat);
	free(philos->actions->clock);
	free(philos->actions->print);
	free(philos->actions->death);
	free(philos->actions->eat);
	free(philos->actions);
	free(philos->info);
	free(philos);
}

int	ft_eat_check(t_philo *philos, int i)
{
	static int	count;
	int			check;

	check = 0;
	pthread_mutex_lock(philos[i].actions->eat);
	if ((philos[count]).eat_check)
		count++;
	if (count == philos[i].info->philo_number)
		check = 1;
	pthread_mutex_unlock(philos[i].actions->eat);
	return (check);
}
