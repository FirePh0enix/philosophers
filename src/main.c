/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 13:36:05 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/21 16:17:52 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

pthread_mutex_t	*alloc_fork()
{
	pthread_mutex_t	*fork;

	fork = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(fork, NULL);
	return (fork);
}

suseconds_t	ms()
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000 + tv.tv_usec) / 1000);
}

void	advanced_sleep(int time)
{
	size_t	start;

	start = ms();
	while ((ms() - start) < time)
		usleep(500);
}

void	init_forks(t_global *global)
{
	t_philo	*philos;
	int		i;

	philos = global->philos;
	if (global->num == 1)
	{
		philos[0].right_fork = alloc_fork();
		return ;
	}
	philos[0].right_fork = alloc_fork();
	i = 1;
	while (i < global->num)
	{
		philos[i].right_fork = alloc_fork();
		philos[i].left_fork = philos[i - 1].right_fork;
		i++;
	}
	philos[0].left_fork = philos[global->num - 1].right_fork;
}

void	init_philos(t_global *global)
{
	int	i;

	init_forks(global);
	i = 0;
	while (i < global->num)
	{
		global->philos[i].num = i + 1;
		global->philos[i].global = global;
		global->philos[i].last_meal = ms();
		pthread_mutex_init(&global->philos[i].mutex, NULL);
		i++;
	}
}

// -----------------------------------------------------------------------------
// Routine

bool	is_someone_dead(t_global *global)
{
	bool	b;

	pthread_mutex_lock(&global->print_mutex);
	b = global->someone_dead;
	pthread_mutex_unlock(&global->print_mutex);
	return (b);
}

int	lock_forks(t_philo *philo)
{
	if (philo->num % 2 == 1)
	{
		pthread_mutex_lock(philo->right_fork);
		if (is_someone_dead(philo->global))
		{
			pthread_mutex_unlock(philo->right_fork);
			return (-1);
		}
		printf("%lu %d is taking a fork\n", ms() - philo->global->start_ms, philo->num);
		pthread_mutex_lock(philo->left_fork);
		printf("%lu %d is taking a fork\n", ms() - philo->global->start_ms, philo->num);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		if (is_someone_dead(philo->global))
		{
			pthread_mutex_unlock(philo->left_fork);
			return (-1);
		}
		printf("%lu %d is taking a fork\n", ms() - philo->global->start_ms, philo->num);
		pthread_mutex_lock(philo->right_fork);
		printf("%lu %d is taking a fork\n", ms() - philo->global->start_ms, philo->num);
	}
	return (0);
}

void	do_eat(t_philo *philo)
{
	if (lock_forks(philo) == -1)
		return ;

	if (philo->global->someone_dead)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}

	pthread_mutex_lock(&philo->global->print_mutex);
		printf("%lu %d is eating\n", ms() - philo->global->start_ms, philo->num);
	pthread_mutex_unlock(&philo->global->print_mutex);

	advanced_sleep(philo->global->time_to_sleep);
	pthread_mutex_lock(&philo->mutex);
	philo->last_meal = ms();
	pthread_mutex_unlock(&philo->mutex);

	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	do_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->global->print_mutex);
	printf("%lu %d is sleeping\n", ms() - philo->global->start_ms, philo->num);
	pthread_mutex_unlock(&philo->global->print_mutex);
	advanced_sleep(philo->global->time_to_sleep);
}

void	do_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->global->print_mutex);
	printf("%lu %d is thinking\n", ms() - philo->global->start_ms, philo->num);
	pthread_mutex_unlock(&philo->global->print_mutex);
}

void	*routine(t_philo *philo)
{
	while (1)
	{
		if (is_someone_dead(philo->global))
			break ;
		do_eat(philo);
		if (is_someone_dead(philo->global))
			break ;
		do_sleep(philo);
		if (is_someone_dead(philo->global))
			break ;
		do_think(philo);
	}
	return (NULL);
}

// -----------------------------------------------------------------------------
// Edit

bool	check_if_someone_dead(t_global *global)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < global->num)
	{
		philo = &global->philos[i];
		pthread_mutex_lock(&philo->mutex);
		if (ms() - philo->last_meal >= global->time_to_die)
		{
			pthread_mutex_lock(&global->print_mutex);
			global->someone_dead = true;
			pthread_mutex_unlock(&global->print_mutex);
			printf("%lu %d is dead\n", ms() - philo->global->start_ms, philo->num);
			pthread_mutex_unlock(&philo->mutex);
			return (true);
		}
		pthread_mutex_unlock(&philo->mutex);
		i++;
	}
	return (false);
}

int	main(int argc, char *argv[])
{
	int			philo_count = atoi(argv[1]);
	int			time_to_die = atoi(argv[2]);
	int			time_to_eat = atoi(argv[3]);
	int			time_to_sleep = atoi(argv[4]);
	t_global	*global;

	global = malloc(sizeof(t_global));
	if (!global)
		return (1);
	global->philos = malloc(sizeof(t_philo) * philo_count);
	global->num = philo_count;
	if (!global->philos)
		return (free(global), 1);
	pthread_mutex_init(&global->print_mutex, NULL);
	init_philos(global);
	global->time_to_die = time_to_die;
	global->time_to_eat = time_to_eat;
	global->time_to_sleep = time_to_sleep;

	printf("philos = %d, time to die = %d, time to eat = %d, time to sleep = %d\n", philo_count, time_to_die, time_to_eat, time_to_sleep);

	global->start_ms = ms();
	for (int i = 0; i < global->num; i++)
		pthread_create(&global->philos[i].thread, NULL, (void *) routine, &global->philos[i]);

	while (!check_if_someone_dead(global))
	{
	}

	for (int i = 0; i < global->num; i++)
		pthread_join(global->philos[i].thread, NULL);

	for (int i = 0; i < global->num; i++)
		free(global->philos[i].right_fork);
	free(global->philos);
	free(global);
}
