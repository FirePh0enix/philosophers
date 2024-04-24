/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:56:26 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/24 12:31:54 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	create_threads(t_global *global)
{
	int	i;

	i = 0;
	while (i < global->num)
	{
		pthread_create(&global->philos[i].thread, NULL, (void *) routine,
			&global->philos[i]);
		i++;
	}
	return (true);
}

bool	join_threads(t_global *global)
{
	int	i;

	i = 0;
	while (i < global->num)
	{
		if (!global->philos[i].thread)
		{
			i++;
			continue ;
		}
		if (pthread_join(global->philos[i].thread, NULL) != 0)
			return (false);
		i++;
	}
	return (true);
}

bool	init_philos(t_global *global)
{
	int	i;

	if (!init_forks(global))
		return (false);
	i = 0;
	while (i < global->num)
	{
		global->philos[i].num = i + 1;
		global->philos[i].global = global;
		global->philos[i].last_meal = ms();
		global->philos[i].meal_count = 0;
		pthread_mutex_init(&global->philos[i].mutex, NULL);
		i++;
	}
	return (true);
}
