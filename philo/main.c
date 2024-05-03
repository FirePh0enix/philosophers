/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 13:36:05 by ledelbec          #+#    #+#             */
/*   Updated: 2024/05/03 11:26:22 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool	check_if_someone_dead(t_global *global)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < global->num)
	{
		philo = &global->philos[i];
		pthread_mutex_lock(&philo->mutex);
		pthread_mutex_lock(&global->print_mutex);
		if (ms() - philo->last_meal > global->time_to_die)
		{
			global->someone_dead = true;
			printf("%lu %d is dead\n", ms() - philo->global->start_ms,
				philo->num);
			pthread_mutex_unlock(&global->print_mutex);
			pthread_mutex_unlock(&philo->mutex);
			return (true);
		}
		pthread_mutex_unlock(&global->print_mutex);
		pthread_mutex_unlock(&philo->mutex);
		i++;
	}
	return (false);
}

bool	check_end(t_global *global)
{
	int		i;
	t_philo	*philo;

	if (global->max_meals == -1)
		return (false);
	i = 0;
	while (i < global->num)
	{
		philo = &global->philos[i];
		pthread_mutex_lock(&philo->mutex);
		if (philo->meal_count < global->max_meals)
		{
			pthread_mutex_unlock(&philo->mutex);
			return (false);
		}
		pthread_mutex_unlock(&philo->mutex);
		i++;
	}
	pthread_mutex_lock(&global->print_mutex);
	global->someone_dead = true;
	pthread_mutex_unlock(&global->print_mutex);
	return (true);
}

bool	parse_args(int argc, char *argv[], t_global *global)
{
	if (argc == 5)
	{
		if (!ft_atol_err(argv[1], &global->num)
			|| !ft_atol_err(argv[2], &global->time_to_die)
			|| !ft_atol_err(argv[3], &global->time_to_eat)
			|| !ft_atol_err(argv[4], &global->time_to_sleep))
			return (false);
	}
	else if (argc == 6)
	{
		if (!ft_atol_err(argv[1], &global->num)
			|| !ft_atol_err(argv[2], &global->time_to_die)
			|| !ft_atol_err(argv[3], &global->time_to_eat)
			|| !ft_atol_err(argv[4], &global->time_to_sleep)
			|| !ft_atol_err(argv[5], &global->max_meals))
			return (false);
	}
	else
		return (false);
	if (global->num < 1)
		return (false);
	return (true);
}

static bool	init_and_loop(t_global *global)
{
	if (!init_philos(global))
		return (false);
	pthread_mutex_init(&global->print_mutex, NULL);
	global->start_ms = ms();
	if (!create_threads(global))
	{
		join_threads(global);
		return (false);
	}
	while (1)
	{
		if (check_end(global))
			break ;
		if (check_if_someone_dead(global))
			break ;
	}
	join_threads(global);
	free_all(global);
	return (true);
}

int	main(int argc, char *argv[])
{
	t_global	*global;

	global = malloc(sizeof(t_global));
	if (!global)
		return (1);
	memset(global, 0, sizeof(t_global));
	global->max_meals = -1;
	if (!global)
		return (1);
	if (!parse_args(argc, argv, global))
		return (free(global), 0);
	global->philos = malloc(sizeof(t_philo) * global->num);
	if (!global->philos)
		return (free(global), 1);
	memset(global->philos, 0, sizeof(t_philo) * global->num);
	if (!init_and_loop(global))
		return (free_all(global), 1);
}
