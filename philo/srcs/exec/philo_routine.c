/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 14:56:59 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/31 11:26:41 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

// to fill now variable that would be used for timestamp

time_t	fill_now_print(t_settings *set)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec - set->subunit) * 1000000 + (tv.tv_usec - set->subusec));
}

// handle everything related to the forks availability
// waiting for it to be available and setup the bool

void	routine_take_fork(t_philo *philo, bool right)
{
	if (right)
	{
		pthread_mutex_lock(&philo->right->mutex);
		while (!philo->right->available)
		{
			pthread_mutex_unlock(&philo->right->mutex);
			usleep(10);
			pthread_mutex_lock(&philo->right->mutex);
		}
		philo->right->available = false;
		pthread_mutex_unlock(&philo->right->mutex);
	}
	else
	{
		pthread_mutex_lock(&philo->left->mutex);
		while (!philo->left->available)
		{
			pthread_mutex_unlock(&philo->left->mutex);
			usleep(10);
			pthread_mutex_lock(&philo->left->mutex);
		}
		philo->left->available = false;
		pthread_mutex_unlock(&philo->left->mutex);
	}
	print_msg_routine(philo, IS_TAKING_FORK);
}

// reset time_alive and add a + 1 to meal count

static void	update_eat(t_philo *philo)
{
	struct timeval	tv;
	time_t			now;

	philo->meals_eaten = philo->meals_eaten + 1;
	gettimeofday(&tv, NULL);
	now = tv.tv_sec * 1000000 + tv.tv_usec;
	pthread_mutex_lock(&philo->t_alive_mutex);
	philo->t_alive = now;
	pthread_mutex_unlock(&philo->t_alive_mutex);
	return ;
}

// here this handle all the printing msg
// related to the project, as eat sleep
// forks think... it also reset the time
// alice of philos right bfr printing the eating
// msg

void	print_msg_routine(t_philo *philo, size_t cases)
{
	time_t	now;

	pthread_mutex_lock(&philo->set->print_mutex);
	if (cases == IS_EATING && philo->set->death != true)
	{
		update_eat(philo);
		now = fill_now_print(philo->set);
		printf("%ld %ld is eating\n", now / 1000, philo->id);
	}
	else if (cases == IS_THINKING && philo->set->death != true)
	{
		now = fill_now_print(philo->set);
		printf("%ld %ld is thinking\n", now / 1000, philo->id);
	}
	else if (cases == IS_TAKING_FORK && philo->set->death != true)
	{
		now = fill_now_print(philo->set);
		printf("%ld %ld has taken a fork\n", now / 1000, philo->id);
	}
	else if (cases == IS_SLEEPING && philo->set->death != true)
	{
		now = fill_now_print(philo->set);
		printf("%ld %ld is sleeping\n", now / 1000, philo->id);
	}
	pthread_mutex_unlock(&philo->set->print_mutex);
}
