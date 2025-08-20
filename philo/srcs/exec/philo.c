/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:08:41 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/18 15:44:28 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <sys/time.h> //gettingtimeofdayneedit
#include <unistd.h>   //usleep

static long	fill_now(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}

static int	creating_thread(t_philo *philo, bool even, long now)
{
	while (philo)
	{
		now = fill_now();
		pthread_mutex_lock(&philo->t_alive_mutex);
		philo->t_alive = now;
		pthread_mutex_unlock(&philo->t_alive_mutex);
		if (even)
		{
			if (pthread_create(&philo->thread_id, NULL, &routine_even,
					philo) != 0)
				return (RETURN_FAILURE);
			even = false;
		}
		else
		{
			if (pthread_create(&philo->thread_id, NULL, &routine_odd,
					philo) != 0)
				return (RETURN_FAILURE);
			even = true;
		}
		philo = philo->next;
	}
	return (RETURN_SUCCESS);
}

static int	create_thread(t_philo *philo, t_philo *tmp, bool even)
{
	tmp = philo;
	creating_thread(philo, even, 0);
	if (pthread_create(&tmp->set->monitor_thread_id, NULL, &philo_monitor,
			tmp) != 0)
		return (RETURN_FAILURE);
	return (RETURN_SUCCESS);
}

static int	join_thread(t_philo *philo)
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
// protect create_thread + retour positif si success et aussi protect join?

void	philosopher(t_settings *set, t_philo *philo)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	set->subunit = tv.tv_sec;
	set->subusec = tv.tv_usec;
	gettimeofday(&tv, NULL); //secure?
	set->time_passed = (tv.tv_sec - set->subunit) * 1000000 + (tv.tv_usec
			- set->subusec);
	create_thread(philo, NULL, true);
	join_thread(philo);
}
