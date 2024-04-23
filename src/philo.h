/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ledelbec <ledelbec@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 13:43:08 by ledelbec          #+#    #+#             */
/*   Updated: 2024/04/23 13:00:54 by ledelbec         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <pthread.h>
# include <sys/select.h>
# include <stdbool.h>

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

	int				meal_count;
}	t_philo;

typedef struct s_global
{
	t_philo			*philos;
	long			num;
	pthread_mutex_t	print_mutex;
	bool			someone_dead;

	suseconds_t		time_to_eat;
	suseconds_t		time_to_sleep;
	suseconds_t		time_to_die;

	suseconds_t		start_ms;

	long			max_meals;
}	t_global;

suseconds_t	ms(void);
void		advanced_sleep(int time);

int			lock_forks(t_philo *philo);
void		init_forks(t_global *global);

void		print_msg(t_philo *philo, suseconds_t time, char *s);

void		init_philos(t_global *global);
void		*routine(t_philo *philo);

bool		ft_atol_err(char *s, long *l);
void		free_all(t_global *global);

void		create_threads(t_global *global);
void		join_threads(t_global *global);

#endif
