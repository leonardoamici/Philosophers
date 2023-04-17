/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 15:57:16 by lamici            #+#    #+#             */
/*   Updated: 2023/04/14 10:20:41 by lamici           ###   ########.fr       */
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
    int     die_time;
    int     eat_ammount;
    int     philo_number;
    unsigned long long   conception;
    pthread_mutex_t *clock;
    pthread_mutex_t *print;
}              t_info;

typedef struct  s_philo
{
    t_info  *info;
    pthread_t    philo;
    int     id;
    pthread_mutex_t *left;
    pthread_mutex_t *right;
    int     telapsed;
}t_philo;

int	    ft_atoi(const char *str);
int	    ft_isdigit(int c);
int	    ft_error_deal(int ac, char **av);
void	ft_intset(int **arr, int value, int size);

#endif
