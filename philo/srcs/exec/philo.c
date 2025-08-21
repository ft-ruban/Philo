/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:08:41 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/21 16:40:47 by ldevoude         ###   ########lyon.fr   */
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

static int	creating_philo_thread(t_philo *philo, bool even, t_philo *head)
{	
	while (philo)
	{
		if (even)
		{
			if (pthread_create(&philo->thread_id, NULL, &routine_even,
					philo) != 0)
				return(RETURN_FAILURE);
			even = false;
		}
		else
		{
			if (pthread_create(&philo->thread_id, NULL, &routine_odd,
					philo) != 0)
				return(RETURN_FAILURE);
			even = true;
		}
		philo = philo->next;
	}
	return(RETURN_SUCCESS);
}
static int begin_timestamp(t_settings *set)
{
	struct timeval	tv;
	
	pthread_mutex_lock(&set->print_mutex);
	set->start = true;
	if(gettimeofday(&tv, NULL))
		return(RETURN_FAILURE);
	set->subunit = tv.tv_sec;
	set->subusec = tv.tv_usec;
	if(gettimeofday(&tv, NULL))
		return(RETURN_FAILURE);
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
	if(creating_philo_thread(philo, even, philo))
		return(cleanup_threads_on_error(philo, philo));                                                                 
	if(begin_timestamp(philo->set))
		return(cleanup_threads_on_error(philo, philo));
	if (pthread_create(&tmp->set->monitor_thread_id, NULL, &philo_monitor,
			tmp))
	{
		while (philo)
		{
			if (pthread_join(philo->thread_id, NULL))
				return (RETURN_FAILURE);
			philo = philo->next;
		}
		return(RETURN_FAILURE);
	}
	if (wait_all_thread(philo))
		return(RETURN_FAILURE);
	return (RETURN_SUCCESS);
	
		while(tmp != philo) //TODO pour demain de regler la nouvelle logique flemme dodo
	{
		if (pthread_join(tmp->thread_id, NULL) != 0)
			return(RETURN_FAILURE);
		tmp = tmp->next;
	}
	return(RETURN_FAILURE);
}

// protect create_thread + retour positif si success et aussi protect join?

// int	philosopher(t_settings *set, t_philo *philo)
// {
// 	if (prepare_creation_thread(philo, NULL, true))
// 		return (RETURN_FAILURE);

// 	return(RETURN_SUCCESS);
// }
