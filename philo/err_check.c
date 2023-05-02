/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lamici <lamici@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 14:19:31 by lamici            #+#    #+#             */
/*   Updated: 2023/05/02 10:01:31 by lamici           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_int_limit_check(char *str)
{
	long long	res;
	int			sign;
	int			i;
	int			dign;	

	dign = 0;
	res = 0;
	sign = 1;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9' && dign <= 11)
	{
		res = res * 10 + str[i] - '0';
		i++;
		dign++;
	}
	if (dign > 10 || res * sign > INT_MAX || res * sign < INT_MIN)
		return (1);
	return (0);
}

int	ft_int_type_check(char *str)
{
	int	i;
	int	check;

	check = 0;
	if (str[0] == '-' || str[0] == '+' || ft_isdigit(str[0]))
	{
		if (str[1] == '\0' && (str[0] == '+' || str[0] == '-'))
			return (1);
		i = 1;
		while (str[i] && check == 0)
		{
			if (!ft_isdigit(str[i]))
				check = 1;
			i++;
		}
	}
	else
		check = 1;
	return (check);
}

int	ft_error_deal(int ac, char **av)
{
	int	check;
	int	j;

	check = 0;
	j = 0;
	while (j < ac && !check)
	{
		check += ft_int_type_check(av[j]);
		check += ft_int_limit_check(av[j]);
		if (atoi(av[j]) < 0)
			check++;
		j++;
	}
	return (check);
}
