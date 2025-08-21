/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 18:24:01 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/21 10:19:15 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <sys/time.h>
#include <unistd.h> //usleep

//duplicata dans philo_routine
static long	fill_now_variable(long *now)
{
	struct timeval	tv;

	if(gettimeofday(&tv, NULL))
		return(RETURN_FAILURE);
	*now = (tv.tv_sec * 1000000 + tv.tv_usec);
	return (RETURN_SUCCESS);
}

static int	philo_monitor_loop_threads(t_philo *tmp, long now)
{
	struct timeval	tv;

	while (tmp)
	{
		pthread_mutex_lock(&tmp->t_alive_mutex);
		fill_now_variable(&now);
		if (now - tmp->t_alive > tmp->set->t_die
			&& tmp->meals_eaten != tmp->set->max_meal)
		{
			pthread_mutex_unlock(&tmp->t_alive_mutex);
			pthread_mutex_lock(&tmp->set->print_mutex);
			if (!tmp->set->death)
			{
				gettimeofday(&tv, NULL);
				tmp->set->time_passed = (tv.tv_sec - tmp->set->subunit)
					* 1000000 + (tv.tv_usec - tmp->set->subusec);
				pthread_mutex_lock(&tmp->set->death_mutex);
				tmp->set->death = true;
				pthread_mutex_unlock(&tmp->set->death_mutex);
				printf("%ld %ld died\n", tmp->set->time_passed / 1000, tmp->id);
			}
			pthread_mutex_unlock(&tmp->set->print_mutex);
			return (1);
		}
		pthread_mutex_unlock(&tmp->t_alive_mutex);
		tmp = tmp->next;
	}
	return (RETURN_SUCCESS);
}

void	*philo_monitor(void *arg)
{
	t_philo			*tmp;
	t_philo			*start;
	long			now;

	start = arg;
	tmp = arg;
	usleep(tmp->set->t_die);
	pthread_mutex_lock(&start->set->pasta_mutex);
	while (!start->set->death
		&& start->set->philo_full_pasta != start->set->nbr_philo)
	{
		pthread_mutex_unlock(&start->set->pasta_mutex);
		usleep(1000);
		tmp = start;
		now = 0;
		if (philo_monitor_loop_threads(tmp, now))
			return (NULL);
		//usleep(tmp->set->t_die / 2);
		//usleep(tmp->set->t_die);
		
		pthread_mutex_lock(&start->set->pasta_mutex);
	}
	pthread_mutex_unlock(&start->set->pasta_mutex);
	return (NULL);
}
