/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:51:08 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/23 12:52:04 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

suseconds_t	ms(void);
void		advanced_sleep(int time);
int			lock_forks(t_philo *philo);
void		print_msg(t_philo *philo, suseconds_t time, char *s);

bool	is_someone_dead(t_global *global)
{
	bool	b;

	pthread_mutex_lock(&global->print_mutex);
	b = global->someone_dead;
	pthread_mutex_unlock(&global->print_mutex);
	return (b);
}

void	do_eat(t_philo *philo)
{
	if (lock_forks(philo) == -1)
		return ;
	if (is_someone_dead(philo->global))
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	print_msg(philo, ms(), "is eating");
	pthread_mutex_lock(&philo->mutex);
	philo->last_meal = ms();
	philo->meal_count++;
	pthread_mutex_unlock(&philo->mutex);
	advanced_sleep(philo->global->time_to_sleep);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

void	*routine(t_philo *philo)
{
	if (philo->global->num == 1)
		advanced_sleep(philo->global->time_to_die);
	while (1)
	{
		if (is_someone_dead(philo->global))
			break ;
		do_eat(philo);
		if (is_someone_dead(philo->global))
			break ;
		print_msg(philo, ms(), "is sleeping");
		advanced_sleep(philo->global->time_to_sleep);
		if (is_someone_dead(philo->global))
			break ;
		print_msg(philo, ms(), "is thinking");
	}
	return (NULL);
}
