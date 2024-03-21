/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 12:02:57 by ledelbec          #+#    #+#             */
/*   Updated: 2024/03/21 23:22:22 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct s_global
{
	pthread_mutex_t	mutex;
	bool			someone_dead;
}	t_global;

typedef struct s_philo
{
	int				num;
	pthread_mutex_t	mutex;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;

	suseconds_t		last_meal;

	suseconds_t		time_to_eat;
	suseconds_t		time_to_sleep;

	t_global		*global;
}	t_philo;

void	do_something(t_philo *philo);

suseconds_t	whats_the_time()
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000 + tv.tv_usec) / 1000);
}

pthread_mutex_t	*give_me_a_new_fork()
{
	pthread_mutex_t	*fork;

	fork = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(fork, NULL);
	return (fork);
}

void	init_forks(t_philo *philos, int count)
{
	int	i;

	if (count == 1)
	{
		philos[0].right_fork = give_me_a_new_fork();
		return ;
	}
	philos[0].right_fork = give_me_a_new_fork();
	i = 1;
	while (i < count)
	{
		philos[i].right_fork = give_me_a_new_fork();
		philos[i].left_fork = philos[i - 1].right_fork;
		i++;
	}
	philos[0].left_fork = philos[count - 1].right_fork;
}

void	lets_the_simulation_begin(t_philo *philos, int count)
{
	int	i;

	i = -1;
	while (++i < count)
	{
		pthread_create(&philos[i].thread, NULL, (void *)do_something, philos + i);
	}
}

void	do_something(t_philo *philo)
{
	while (!philo->global->someone_dead)
	{
		/*
		 * The philo is trying to take its forks...
		 */
		pthread_mutex_lock(&philo->mutex);
			pthread_mutex_lock(philo->right_fork);
			printf("%lu %d is taking a fork\n", whats_the_time(), philo->num);
			pthread_mutex_lock(philo->left_fork);
			printf("%lu %d is taking a fork\n", whats_the_time(), philo->num);
		pthread_mutex_unlock(&philo->mutex);

		/*
		 * The philo is eating...
		 */
		pthread_mutex_lock(&philo->mutex);
			printf("%lu %d is eating\n", whats_the_time(), philo->num);
			usleep(philo->time_to_eat * 1000);
			philo->last_meal = whats_the_time();
		pthread_mutex_unlock(&philo->mutex);

		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);

		/*
		 * The philo is sleeping...
		 */
		if (philo->global->someone_dead)
			break ;
		printf("%lu %d is sleeping\n", whats_the_time(), philo->num);
		usleep(philo->time_to_sleep * 1000);

		/*
		 * The philo is thinking...
		 */
		printf("%lu %d is thinking\n", whats_the_time(), philo->num);
	}
}

int	main(int argc, char *argv[])
{
	int	num	= atoi(argv[1]);
	int	time_to_die = atoi(argv[2]);
	int	time_to_eat = atoi(argv[3]);
	int	time_to_sleep = atoi(argv[4]);

	t_philo *philos = malloc(sizeof(t_philo) * num);
	memset(philos, 0, sizeof(t_philo) * num);

	t_global	*global = malloc(sizeof(t_global));
	global->someone_dead = false;
	pthread_mutex_init(&global->mutex, NULL);

	for (int i = 0; i < num; i++)
	{
		philos[i].num = i + 1;
		philos[i].last_meal = whats_the_time();
		philos[i].global = global;
		philos[i].time_to_eat = time_to_eat;
		philos[i].time_to_sleep = time_to_sleep;
		pthread_mutex_init(&philos[i].mutex, NULL);
	}

	init_forks(philos, num);
	lets_the_simulation_begin(philos, num);

	while (!global->someone_dead)
	{
		/*
		 * Check if one of the philo has died.
		 */
		for (int i = 0; i < num; i++)
		{
			//pthread_mutex_lock(&philos[i].mutex);
			if (whats_the_time() - philos[i].last_meal >= time_to_die)
			{
				global->someone_dead = true;
				printf("%lu %d has died\n", whats_the_time(), philos[i].num);
				break;
			}
			//pthread_mutex_unlock(&philos[i].mutex);
		}
	}

	for (int i = 0; i < num; i++)
		pthread_join(philos[i].thread, NULL);

	for (int i = 0; i < num; i++)
	{
		pthread_mutex_destroy(philos[i].right_fork);
		free(philos[i].right_fork);
	}

	pthread_mutex_destroy(&global->mutex);
	free(global);
	free(philos);
}
