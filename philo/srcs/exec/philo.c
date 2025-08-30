/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:08:41 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/30 14:32:12 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

// in case of error we need to make our philo leave
// by setting up the right bools then by waiting all of our philo
// to leave until the count i = to count

static int	cleanup_threads_on_error(t_philo *philo, size_t count)
{
	size_t	i;

	pthread_mutex_lock(&philo->set->print_mutex);
	philo->set->start = true;
	philo->set->death = true;
	pthread_mutex_unlock(&philo->set->print_mutex);
	i = 0;
	while (philo && i < count)
	{
		if (pthread_join(philo->thread_id, NULL))
			return (RETURN_FAILURE);
		philo = philo->next;
		i++;
	}
	return (RETURN_FAILURE);
}

// wait for our monitor to finish its job, then we wait for all
// philo to leave one by one until all of them are terminated

static int	wait_all_thread(t_philo *philo)
{
	if (pthread_join(philo->set->monitor_thread_id, NULL))
		return (RETURN_FAILURE);
	while (philo)
	{
		if (pthread_join(philo->thread_id, NULL))
			return (RETURN_FAILURE);
		philo = philo->next;
	}
	return (RETURN_SUCCESS);
}

// start out timestamp and tell our philo that
// they can start their own routine once
// mutex print is unlocked they all now can leave
// their waiting loop

static int	begin_timestamp(t_settings *set)
{
	struct timeval	tv;

	pthread_mutex_lock(&set->print_mutex);
	set->start = true;
	if (gettimeofday(&tv, NULL))
		return (RETURN_FAILURE);
	set->subunit = tv.tv_sec;
	set->subusec = tv.tv_usec;
	if (gettimeofday(&tv, NULL))
		return (RETURN_FAILURE);
	set->time_passed = (tv.tv_sec - set->subunit) * 1000000 + (tv.tv_usec
			- set->subusec);
	pthread_mutex_unlock(&set->print_mutex);
	return (RETURN_SUCCESS);
}

// create our philo and direct them at the right place
// depending of if they are an even or true ID number
// because the logic depend of their emplacement later
// in the routines count is used for cleanup to clean
// the right amount of thread

static int	creating_philo_thread(t_philo *philo, bool even, size_t *count)
{
	while (philo)
	{
		if (even) // changer debile
		{
			if (pthread_create(&philo->thread_id, NULL, &routine_odd, philo))
				return (RETURN_FAILURE);
			even = false;
		}
		else
		{
			if (pthread_create(&philo->thread_id, NULL, &routine_even, philo))
				return (RETURN_FAILURE);
			even = true;
		}
		*count = *count + 1;
		philo = philo->next;
	}
	return (RETURN_SUCCESS);
}

// here we get the threads creation ready first we create
// a thread for each philo (if at any point there is a error
// we use cleanup_thread_on_error to leave properly)
// then we start our timestamp and set our start bool at true
// to tell our waiting philos that they can start their routine
// to make sure everything get started when all philos are created
// then we create our monitor and finaly we wait for all of them
// to finish their things

int	prepare_creation_thread(t_philo *philo, t_philo *tmp, bool even)
{
	size_t	count;

	count = 0;
	if (creating_philo_thread(philo, even, &count))
		return (cleanup_threads_on_error(philo, count));
	if (begin_timestamp(philo->set))
		return (cleanup_threads_on_error(philo, count));
	if (pthread_create(&tmp->set->monitor_thread_id, NULL, &philo_monitor, tmp))
		return (cleanup_threads_on_error(philo, count));
	if (wait_all_thread(philo))
		return (RETURN_FAILURE);
	return (RETURN_SUCCESS);
}
