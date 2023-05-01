/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 22:47:43 by leo               #+#    #+#             */
/*   Updated: 2023/05/01 20:42:10 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void    ft_eat(t_philo *philo)
{
    pthread_mutex_lock(philo->left);
	ft_print("has taken left fork\n", philo);
	pthread_mutex_lock(philo->right);
	ft_print("has taken right fork\n", philo);
	pthread_mutex_lock(philo->actions->death);
	philo->telapsed = ft_clock();
	pthread_mutex_unlock(philo->actions->death);
	ft_print("is eating\n", philo);
	usleep(1000 * philo->info->eat_time);
	pthread_mutex_unlock(philo->left);
	pthread_mutex_unlock(philo->right);
}

void	lonely_boy(void *args)
{
	t_philo		*philo;
	int		eat_ammount;
	
	philo = (t_philo *)vargp;
	eat_ammount = philo->info->eat_ammount;
	if(eat_ammount != 0)
	{
		ft_print("boy i sure am hungry\n", philo);
		ft_print("looks like i only own a singular fork\n", philo);
		usleep(1000 * philo->info->eat_time);
		ft_print("dies of cringe\n", philo);
	}
	else
		ft_print("no need to eat im already full of knowledge\n", philo);

}