/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 18:24:01 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/20 16:25:24 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <sys/time.h>
#include <unistd.h> //usleep

static int	philo_monitor_loop_threads(t_philo *tmp, long now,
		struct timeval tv)
{
	while (tmp)
	{
		pthread_mutex_lock(&tmp->t_alive_mutex);
		if (now - tmp->t_alive > tmp->set->t_die
			&& tmp->meals_eaten != tmp->set->max_meal)
		{
			pthread_mutex_unlock(&tmp->t_alive_mutex);
			pthread_mutex_lock(&tmp->set->print_mutex);
			if (!tmp->set->death)
			{
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
	struct timeval	tv;

	start = arg;
	tmp = arg;
	//usleep(tmp->set->t_die / 2);
	pthread_mutex_lock(&start->set->pasta_mutex);
	while (!start->set->death
		&& start->set->philo_full_pasta != start->set->nbr_philo)
	{
		pthread_mutex_unlock(&start->set->pasta_mutex);
		usleep(5000);
		tmp = start;
		gettimeofday(&tv, NULL);
		now = tv.tv_sec * 1000000 + tv.tv_usec;
		if (philo_monitor_loop_threads(tmp, now, tv))
			return (NULL);
		//usleep(tmp->set->t_die / 2);
		//usleep(tmp->set->t_die);
		
		pthread_mutex_lock(&start->set->pasta_mutex);
	}
	pthread_mutex_unlock(&start->set->pasta_mutex);
	return (NULL);
}
