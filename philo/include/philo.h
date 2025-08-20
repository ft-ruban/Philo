/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 15:16:55 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/20 14:16:21 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
//#include <unistd.h>
# include <pthread.h>
# include <stdbool.h>

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define RETURN_SUCCESS 0
# define RETURN_FAILURE 1

typedef enum e_activity_philosophers
{
	TAKEN_A_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED,
	WAIT
}						t_activity_philosophers;

typedef enum e_return_value_main
{
	MALLOC_SET_ERROR = 1,
	PARSING_ERROR,
	MALLOC_PHILO_OR_FORK_ERROR,
	SETUP_STRUCT_ERROR,
	EXEC_ERROR,
	DESTROY_MUTEX_ERROR,
}						t_return_value_main;

typedef struct s_forks
{
	size_t				id;
	bool				available;
	bool				bool_mutex;
	pthread_mutex_t		mutex;
	struct s_forks		*next;
}						t_forks;

typedef struct s_philo
{
	size_t				id;
	long				meals_eaten;
	long				t_alive;
	bool				bool_alive_mutex;
	pthread_mutex_t		t_alive_mutex;
	struct s_forks		*left;
	struct s_forks		*right;
	struct s_philo		*next;
	struct s_settings	*set;
	pthread_t			thread_id;
}						t_philo;

typedef struct s_settings
{
	pthread_t			monitor_thread_id;
	long				nbr_philo;
	long				t_die;
	long				t_eat;
	long				t_sleep;
	time_t				subunit;
	time_t				subusec;
	time_t				time_passed;
	long				philo_full_pasta;
	long				max_meal;
	pthread_mutex_t		pasta_mutex;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		print_mutex;
	bool				nbr_philo_odd;
	bool				bool_pasta_mutex;
	bool				bool_death_mutex;
	bool				bool_print_mutex;
	bool				death;
}						t_settings;

// utils.c
int						setup_philo_forks_struct(t_settings *settings,
							t_philo *philo, t_forks *forks);
int						free_structs(t_settings *set, t_philo *philo,
							t_forks *forks, int return_value);
int 					destroy_mutex_nodes(t_philo *philo, t_forks *forks, long i);
int						destroy_mutex_fail(t_settings *settings, t_philo *philo, t_forks *forks,
							long i);

#endif