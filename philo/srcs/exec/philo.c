/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:08:41 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/21 12:23:34 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <sys/time.h> //gettingtimeofdayneedit
#include <unistd.h>   //usleep

//may be obsolete here
// static long	fill_now_variable(long *now)
// {
// 	struct timeval	tv;

// 	if(gettimeofday(&tv, NULL))
// 		return(RETURN_FAILURE);
// 	*now = (tv.tv_sec * 1000000 + tv.tv_usec);
// 	return (RETURN_SUCCESS);
// }

static int cleanup_threads_on_error(t_philo *philo, t_philo *head)
{
	t_philo *tmp;

	tmp =  head;
	while(tmp != philo)
	{
		if (pthread_join(tmp->thread_id, NULL) != 0)
			return(RETURN_FAILURE);
		tmp = tmp->next;
	}
	return(RETURN_FAILURE);
}

static int	creating_thread(t_philo *philo, bool even, t_philo *head)
{
	struct timeval	tv;
	
	head = philo;
	while (philo)
	{
		if (even)
		{
			if (pthread_create(&philo->thread_id, NULL, &routine_even,
					philo) != 0)
				return(cleanup_threads_on_error(philo, head));
			even = false;
		}
		else
		{
			if (pthread_create(&philo->thread_id, NULL, &routine_odd,
					philo) != 0)
				return(cleanup_threads_on_error(philo, head));
			even = true;
		}
		philo = philo->next;
	}
	pthread_mutex_lock(&head->set->print_mutex);
	head->set->start = true;
	if(gettimeofday(&tv, NULL))
		return(RETURN_FAILURE);
	head->set->subunit = tv.tv_sec;
	head->set->subusec = tv.tv_usec;
	if(gettimeofday(&tv, NULL))
		return(RETURN_FAILURE);
	head->set->time_passed = (tv.tv_sec - head->set->subunit) * 1000000 + (tv.tv_usec
			- head->set->subusec);
	pthread_mutex_unlock(&head->set->print_mutex);
	return (RETURN_SUCCESS);
}

static int	prepare_creation_thread(t_philo *philo, t_philo *tmp, bool even)
{
	tmp = philo;
	if(creating_thread(philo, even, philo))
		return(RETURN_FAILURE);
	if (pthread_create(&tmp->set->monitor_thread_id, NULL, &philo_monitor,
			tmp) != 0)
	{
		while (philo)
		{
			if (pthread_join(philo->thread_id, NULL))
				return (RETURN_FAILURE);
			philo = philo->next;
		}
		return(RETURN_FAILURE);
	}
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
// protect create_thread + retour positif si success et aussi protect join?

int	philosopher(t_settings *set, t_philo *philo)
{
	set->bool_death_mutex = true;
	if (prepare_creation_thread(philo, NULL, true))
		return (RETURN_FAILURE);
	if (wait_all_thread(philo))
		return(RETURN_FAILURE);
	return(RETURN_SUCCESS);
}
