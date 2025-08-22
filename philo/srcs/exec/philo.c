/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:08:41 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/22 10:41:54 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <sys/time.h> //gettingtimeofdayneedit
#include <unistd.h>   //usleep

static int	cleanup_threads_on_error(t_philo *philo, int count)
{
	int	i;

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

static int	creating_philo_thread(t_philo *philo, bool even, int *count)
{
	while (philo)
	{
		if (even)
		{
			if (pthread_create(&philo->thread_id, NULL, &routine_even, philo))
				return (RETURN_FAILURE);
			even = false;
		}
		else
		{
			if (pthread_create(&philo->thread_id, NULL, &routine_odd, philo))
				return (RETURN_FAILURE);
			even = true;
		}
		*count = *count + 1;
		philo = philo->next;
	}
	return (RETURN_SUCCESS);
}

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
int	prepare_creation_thread(t_philo *philo, t_philo *tmp, bool even)
{
	int	count;

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
