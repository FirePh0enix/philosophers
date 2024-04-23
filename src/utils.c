/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:01:36 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/23 13:00:39 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <limits.h>
#include <unistd.h>
#include <sys/time.h>
#include "philo.h"

bool	ft_atol_err(char *s, long *l)
{
	long	sign;
	long	res;

	sign = 1;
	if (*s == '-')
	{
		sign = -1;
		s++;
	}
	else if (*s == '+')
		s++;
	res = 0;
	if (*s == '\0')
		return (false);
	while (*s)
	{
		if (*s < '0' || *s > '9')
			return (false);
		res = res * 10 + (*s - '0');
		if (res * sign > INT_MAX || res * sign < INT_MIN)
			return (false);
		s++;
	}
	*l = res * sign;
	return (true);
}

suseconds_t	ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	advanced_sleep(int time)
{
	suseconds_t	start;

	start = ms();
	while ((ms() - start) < time)
		usleep(500);
}

void	free_all(t_global *global)
{
	int	i;

	i = 0;
	while (i < global->num)
	{
		free(global->philos[i].right_fork);
		i++;
	}
	free(global->philos);
	free(global);
}
