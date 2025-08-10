/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:16:23 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/10 17:16:37 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <sys/time.h> //getting time of day need it
#include <unistd.h>   //usleep

void	routine_take_fork(t_philo *philo, bool right)
{
	if (right)
	{
		pthread_mutex_lock(&philo->right->mutex);
		philo->right->available = false;
		print_msg_routine(philo, IS_TAKING_FORK);
	}
	else
	{
		pthread_mutex_lock(&philo->left->mutex);
		philo->left->available = false;
		print_msg_routine(philo, IS_TAKING_FORK);
	}
}

static time_t	fill_now_print(t_settings *set)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec - set->subunit) * 1000000 + (tv.tv_usec - set->subusec));
}

static void	update_eat(t_philo *philo)
{
	struct timeval	tv;

	philo->meals_eaten = philo->meals_eaten + 1;
	pthread_mutex_lock(&philo->t_alive_mutex);
	gettimeofday(&tv, NULL);
	philo->t_alive = tv.tv_sec * 1000000 + tv.tv_usec;
	pthread_mutex_unlock(&philo->t_alive_mutex);
	return ;
}

void	print_msg_routine(t_philo *philo, size_t cases)
{
	time_t	now;

	pthread_mutex_lock(&philo->set->print_mutex);
	if (cases == IS_EATING && philo->set->death != true)
	{
		now = fill_now_print(philo->set);
		printf("%ld %ld is eating\n", now / 1000, philo->id);
		update_eat(philo);
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
