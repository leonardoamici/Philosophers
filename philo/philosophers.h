/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 15:57:16 by lamici            #+#    #+#             */
/*   Updated: 2023/04/24 14:59:57 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

typedef struct  s_info
{
    int     eat_time;
    int     sleep_time;
    long long unsigned     die_time;
    int     eat_ammount;
    int     philo_number;
    unsigned long long   conception;
}              t_info;

typedef struct s_mutex
{
    pthread_mutex_t		*death;
	pthread_mutex_t		*print;
	pthread_mutex_t		*eat;
    pthread_mutex_t		*clock;
}               t_mutex;

typedef struct  s_philo
{
    t_info   *info;
    pthread_t    philo;
    int     id;
    int     *eat_check;
    int     *death;
    t_mutex     *actions;
    pthread_mutex_t *left;
    pthread_mutex_t *right;
    unsigned long long     telapsed;
}               t_philo;

int	    ft_atoi(const char *str);
int	    ft_isdigit(int c);
int	    ft_error_deal(int ac, char **av);
void	ft_intset(int **arr, int value, int size);
long long unsigned		ft_clock(long long unsigned otime, pthread_mutex_t *clock);
void	ft_print(char *str, t_philo *philo);
void	ft_usleep(int time);
void	ft_detach(t_philo *philos);
int		*val_set(int num);
void	*ft_philo_even(void *vargp);
void	*ft_philo_odd(void *vargp);
void	ft_alt_launch(t_philo *philos);

#endif
