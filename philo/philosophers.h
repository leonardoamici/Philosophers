/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 15:57:16 by lamici            #+#    #+#             */
/*   Updated: 2023/04/30 22:51:04 by leo              ###   ########.fr       */
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
long long unsigned		ft_timer(long long unsigned otime, pthread_mutex_t *clock);
long long unsigned      ft_clock();
void	ft_print(char *str, t_philo *philo);
void	ft_usleep(int time);
int	    ft_isdead(t_philo *philo);
void	ft_detach(t_philo *philos);
int		*val_set(int num);
void	*ft_philo(void *vargp);
void	ft_alt_launch(t_philo *philos);
void	 ft_give_forks(pthread_mutex_t *mutexes, t_philo *philo, int i, t_info *info);
t_mutex		*act_init(void);
t_info	*ft_init_info(int argc, char **argv);
void    ft_eat(t_philo *philo);

#endif
