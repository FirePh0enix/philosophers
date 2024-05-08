/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:32:05 by ledelbec          #+#    #+#             */
/*   Updated: 2024/05/08 11:01:22 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool		is_someone_dead(t_global *global);
void		print_msg(t_philo *philo, suseconds_t time, char *s);
suseconds_t	ms(void);

pthread_mutex_t	*alloc_fork(void)
{
	pthread_mutex_t	*fork;

	fork = malloc(sizeof(pthread_mutex_t));
	if (!fork)
		return (NULL);
	pthread_mutex_init(fork, NULL);
	return (fork);
}

bool	init_forks(t_global *global)
{
	t_philo	*philos;
	int		i;

	philos = global->philos;
	if (global->num == 1)
	{
		philos[0].right_fork = alloc_fork();
		if (!philos[0].right_fork)
			return (false);
		return (true);
	}
	philos[0].right_fork = alloc_fork();
	if (!philos[0].right_fork)
		return (false);
	i = 1;
	while (i < global->num)
	{
		philos[i].right_fork = alloc_fork();
		if (!philos[i].right_fork)
			return (false);
		philos[i].left_fork = philos[i - 1].right_fork;
		i++;
	}
	philos[0].left_fork = philos[global->num - 1].right_fork;
	return (true);
}

static int	lock_both(t_philo *philo, pthread_mutex_t *first,
	pthread_mutex_t *second)
{
	pthread_mutex_lock(first);
	if (is_someone_dead(philo->global))
	{
		pthread_mutex_unlock(first);
		return (-1);
	}
	print_msg(philo, ms(), "has taken a fork");
	pthread_mutex_lock(second);
	if (is_someone_dead(philo->global))
	{
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		return (-1);
	}
	print_msg(philo, ms(), "has taken a fork");
	return (0);
}

int	lock_forks(t_philo *philo)
{
	if (philo->num % 2 == 1)
		return (lock_both(philo, philo->right_fork, philo->left_fork));
	else
		return (lock_both(philo, philo->left_fork, philo->right_fork));
	return (0);
}
