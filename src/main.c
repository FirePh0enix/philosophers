/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 12:02:57 by ledelbec          #+#    #+#             */
/*   Updated: 2024/02/29 16:52:38 by ledelbec         ###   ########.fr       */
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

typedef struct s_fork
{
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_philo
{
	pthread_mutex_t	mutex;
	int				num;
	pthread_t		thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	bool			is_dead;
	bool			is_alone;

	suseconds_t		time_to_eat;
	suseconds_t		time_to_sleep;
	suseconds_t		last_meal;
}	t_philo;

suseconds_t	getms()
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000 + tv.tv_usec) / 1000);
}

void	do_something(t_philo *philo)
{
	while (!philo->is_dead)
	{
		if (!philo->is_alone)
		{
			pthread_mutex_lock(&philo->left_fork->mutex);
			printf("%ld %d has taken a fork\n", getms(), philo->num);
		}
		pthread_mutex_lock(&philo->right_fork->mutex);
		printf("%ld %d has taken a fork\n", getms(), philo->num);
		printf("%ld %d is eating\n", getms(), philo->num);
		usleep(philo->time_to_eat);
		philo->last_meal = getms();
		if (!philo->is_alone)
			pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_unlock(&philo->right_fork->mutex);
		printf("%ld %d is sleeping\n", getms(), philo->num);
		usleep(philo->time_to_sleep);
		printf("%ld %d is thinking\n", getms(), philo->num);
	}
}

t_fork	*create_fork()
{
	t_fork	*fork;

	fork = malloc(sizeof(t_fork));
	pthread_mutex_init(&fork->mutex, NULL);
	return (fork);
}

void	init_forks(t_philo *philos, int count)
{
	int	i;

	if (count == 1)
	{
		philos[0].right_fork = create_fork();
		philos[0].is_alone = true;
		return ;
	}
	philos[0].right_fork = create_fork();
	i = 1;
	while (i < count - 1)
	{
		philos[i].right_fork = create_fork();
		philos[i].left_fork = philos[i - 1].right_fork;
		i++;
	}
	philos[count - 1].right_fork = create_fork();
	philos[count - 1].left_fork = philos[count - 2].right_fork;
	philos[0].left_fork = philos[count - 1].right_fork;
}

bool	is_someone_dead(t_philo *philos, int count, int time_to_die)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_lock(&philos[i].mutex);
		if (getms() - philos[i].last_meal >= time_to_die)
		{
			philos[i].is_dead = true;
			return (true);
		}
		pthread_mutex_unlock(&philos[i].mutex);
	}
	return (false);
}

int	main(int argc, char *argv[])
{
	int			philo_count;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	t_philo		*philos;
	int			i;

	philo_count = atoi(argv[1]);
	time_to_die = atoi(argv[2]);
	time_to_eat = atoi(argv[3]);
	time_to_sleep = atoi(argv[3]);
	philos = malloc(sizeof(t_philo) * philo_count);
	memset(philos, 0, sizeof(t_philo) * philo_count);
	i = 0;
	while (i < philo_count)
	{
		philos[i].num = i + 1;
		philos[i].last_meal = getms();
		philos[i].time_to_eat = time_to_eat;
		philos[i].time_to_sleep = time_to_sleep;
		pthread_mutex_init(&philos[i].mutex, NULL);
		i++;
	}
	init_forks(philos, philo_count);
	i = 0;
	while (i < philo_count)
	{
		pthread_create(&philos[i].thread, NULL, (void *) do_something, &philos[i]);
		i++;
	}
	while (!is_someone_dead(philos, philo_count, time_to_die))
	{
	}
	printf("%ld %d died\n", getms(), philos[0].num); // TODO
}
