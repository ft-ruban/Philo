/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:16:23 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/30 14:32:33 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

long	get_time_in_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000L + tv.tv_usec);
}

void	ft_usleep(long usec, t_settings *set)
{
	long	start;
	long	now;

	start = get_time_in_us();
	pthread_mutex_lock(&set->print_mutex);
	while (!set->death)
	{
		pthread_mutex_unlock(&set->print_mutex);
		now = get_time_in_us();
		if (now - start >= usec)
		{
			pthread_mutex_lock(&set->print_mutex);
			break ;
		}
		usleep(100);
		pthread_mutex_lock(&set->print_mutex);
	}
	pthread_mutex_unlock(&set->print_mutex);
}

// fill the now variable that would be used to
// give right timer of our philo

long	fill_now_variable(long *now)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (RETURN_FAILURE);
	*now = (tv.tv_sec * 1000000 + tv.tv_usec);
	return (RETURN_SUCCESS);
}

// here in that function our thread wait until all threads are
// created once they get we setup their timer

int	wait_all_threads(t_settings *set, t_philo *philo)
{
	long	now;

	now = 0;
	pthread_mutex_lock(&set->print_mutex);
	while (!set->start)
	{
		pthread_mutex_unlock(&set->print_mutex);
		usleep(1);
		pthread_mutex_lock(&set->print_mutex);
	}
	pthread_mutex_unlock(&set->print_mutex);
	if (fill_now_variable(&now))
		return (RETURN_FAILURE);
	pthread_mutex_lock(&philo->t_alive_mutex);
	philo->t_alive = now;
	pthread_mutex_unlock(&philo->t_alive_mutex);
	return (RETURN_SUCCESS);
}
