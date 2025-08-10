/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 14:56:59 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/10 16:03:53 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <unistd.h> //usleep

static void	routine_sleep(t_philo *philo, bool *first_iteration)
{
	print_msg_routine(philo, IS_SLEEPING);
	if (*first_iteration)
	{
		if (philo->set->t_eat + philo->set->t_sleep > philo->set->t_die)
			usleep(philo->set->t_die);
		else
			usleep(philo->set->t_sleep);
		*first_iteration = false;
	}
	else
		usleep(philo->set->t_sleep);
}

static void	think_fork_even(t_philo *philo, bool first_iteration,
		bool *break_loop)
{
	print_msg_routine(philo, IS_THINKING);
	usleep(250);
	routine_take_fork(philo, false);
	if (philo->right->id == philo->left->id)
	{
		pthread_mutex_lock(&philo->set->death_mutex);
		pthread_mutex_unlock(&philo->left->mutex);
		*break_loop = true;
		return ;
	}
	routine_take_fork(philo, true);
	print_msg_routine(philo, IS_EATING);
	if (first_iteration)
	{
		if (philo->set->t_eat > philo->set->t_die)
			usleep(philo->set->t_die);
		else
			usleep(philo->set->t_eat);
	}
	else
		usleep(philo->set->t_eat);
	philo->left->available = true;
	pthread_mutex_unlock(&philo->left->mutex);
	philo->right->available = true;
	pthread_mutex_unlock(&philo->right->mutex);
}

static void	think_fork_odd(t_philo *philo, bool first_iteration)
{
	print_msg_routine(philo, IS_THINKING);
	usleep(250);
	routine_take_fork(philo, true);
	routine_take_fork(philo, false);
	print_msg_routine(philo, IS_EATING);
	if (first_iteration)
	{
		if (philo->set->t_eat > philo->set->t_die)
			usleep(philo->set->t_die);
		else
			usleep(philo->set->t_eat);
	}
	else
		usleep(philo->set->t_eat);
	philo->left->available = true;
	pthread_mutex_unlock(&philo->left->mutex);
	philo->right->available = true;
	pthread_mutex_unlock(&philo->right->mutex);
}

void	*routine_odd(void *arg)
{
	t_philo	*philo;
	bool	first_iteration;

	philo = (t_philo *)arg;
	first_iteration = true;
	usleep(philo->set->t_eat / 2);
	pthread_mutex_lock(&philo->set->death_mutex);
	while (!philo->set->death && philo->meals_eaten != philo->set->max_meal)
	{
		pthread_mutex_unlock(&philo->set->death_mutex);
		think_fork_odd(philo, first_iteration);
		routine_sleep(philo, &first_iteration);
		pthread_mutex_lock(&philo->set->death_mutex);
	}
	pthread_mutex_unlock(&philo->set->death_mutex);
	pthread_mutex_lock(&philo->set->pasta_mutex);
	if (philo->meals_eaten == philo->set->max_meal)
		philo->set->philo_full_pasta = philo->set->philo_full_pasta + 1;
	pthread_mutex_unlock(&philo->set->pasta_mutex);
	return (0);
}

void	*routine_even(void *arg)
{
	t_philo	*philo;
	bool	first_iteration;
	bool	break_loop;

	philo = (t_philo *)arg;
	break_loop = false;
	first_iteration = true;
	pthread_mutex_lock(&philo->set->death_mutex);
	while (!philo->set->death && philo->meals_eaten != philo->set->max_meal)
	{
		pthread_mutex_unlock(&philo->set->death_mutex);
		think_fork_even(philo, first_iteration, &break_loop);
		if (break_loop)
			break ;
		routine_sleep(philo, &first_iteration);
		pthread_mutex_lock(&philo->set->death_mutex);
	}
	pthread_mutex_unlock(&philo->set->death_mutex);
	pthread_mutex_lock(&philo->set->pasta_mutex);
	if (philo->meals_eaten == philo->set->max_meal)
		philo->set->philo_full_pasta = philo->set->philo_full_pasta + 1;
	pthread_mutex_unlock(&philo->set->pasta_mutex);
	return (0);
}
