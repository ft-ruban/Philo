/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 18:24:01 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/30 15:59:10 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static int	philo_died_detected(t_philo *tmp)
{
	struct timeval	tv;

	pthread_mutex_lock(&tmp->set->print_mutex);
	if (!tmp->set->death)
	{
		gettimeofday(&tv, NULL);
		tmp->set->time_passed = (tv.tv_sec - tmp->set->subunit) * 1000000
			+ (tv.tv_usec - tmp->set->subusec);
		pthread_mutex_lock(&tmp->set->death_mutex);
		tmp->set->death = true;
		pthread_mutex_unlock(&tmp->set->death_mutex);
		printf("%ld %ld died\n", tmp->set->time_passed / 1000, tmp->id);
	}
	pthread_mutex_unlock(&tmp->set->print_mutex);
	return (1);
}

static int	philo_monitor_loop_threads(t_philo *tmp, long now)
{
	while (tmp)
	{
		pthread_mutex_lock(&tmp->t_alive_mutex);
		fill_now_variable(&now);
		if (now - tmp->t_alive > tmp->set->t_die
			&& tmp->meals_eaten != tmp->set->max_meal)
		{
			pthread_mutex_unlock(&tmp->t_alive_mutex);
			return (philo_died_detected(tmp));
		}
		pthread_mutex_unlock(&tmp->t_alive_mutex);
		tmp = tmp->next;
	}
	return (RETURN_SUCCESS);
}

// here is the monitor that is used to check
// whenever a philo is dead to make everything stop as asked
// it only leave it's loop if all philo ate the right amount of food
// or if a philo died.

void	*philo_monitor(void *arg)
{
	t_philo	*start;
	long	now;

	now = 0;
	start = arg;
	usleep(start->set->t_die);
	pthread_mutex_lock(&start->set->pasta_mutex);
	while (!start->set->death
		&& start->set->philo_full_pasta != start->set->nbr_philo)
	{
		pthread_mutex_unlock(&start->set->pasta_mutex);
		usleep(500);
		if (philo_monitor_loop_threads(start, now))
			return (NULL);
		pthread_mutex_lock(&start->set->pasta_mutex);
	}
	pthread_mutex_unlock(&start->set->pasta_mutex);
	return (NULL);
}
