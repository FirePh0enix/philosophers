/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 13:43:08 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/21 15:39:42 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdlib.h>
#include <pthread.h>
#include <sys/select.h>
#include <stdbool.h>

typedef struct s_global	t_global;

typedef struct s_philo
{
	pthread_t		thread;
	int				num;
	pthread_mutex_t	mutex;

	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;

	suseconds_t		last_meal;
	t_global		*global;
}	t_philo;

typedef struct s_global
{
	t_philo			*philos;
	int				num;
	pthread_mutex_t	print_mutex;
	bool			someone_dead;

	suseconds_t		time_to_eat;
	suseconds_t		time_to_sleep;
	suseconds_t		time_to_die;

	suseconds_t		start_ms;
}	t_global;

#endif
