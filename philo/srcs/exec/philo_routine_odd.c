/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_odd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:52:03 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/30 15:57:57 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

// here odd philo take their left forks then right
// also quick checker in case we just have a single philo
// bcs in that case forks id left and right is the same
// eat, and make there forks available once more.

static int	fork_eat_odd(t_philo *philo)
{
	routine_take_fork(philo, false);
	if (philo->right->id == philo->left->id)
		return (SOLO_PHILO);
	routine_take_fork(philo, true);
	print_msg_routine(philo, IS_EATING);
	usleep(philo->set->t_eat);
	pthread_mutex_lock(&philo->left->mutex);
	philo->left->available = true;
	pthread_mutex_unlock(&philo->left->mutex);
	pthread_mutex_lock(&philo->right->mutex);
	philo->right->available = true;
	pthread_mutex_unlock(&philo->right->mutex);
	return (RETURN_SUCCESS);
}

// this is the start for our odd_philo routine.

static int	routine_odd_loop(t_philo *philo)
{
	print_msg_routine(philo, IS_THINKING);
	usleep(philo->set->t_eat / 6);
	pthread_mutex_lock(&philo->set->death_mutex);
	while (!philo->set->death && philo->meals_eaten != philo->set->max_meal)
	{
		pthread_mutex_unlock(&philo->set->death_mutex);
		if (fork_eat_odd(philo))
			return (SOLO_PHILO);
		print_msg_routine(philo, IS_SLEEPING);
		usleep(philo->set->t_sleep);
		print_msg_routine(philo, IS_THINKING);
		if (!philo->set->nbr_philo_odd)
			usleep(philo->set->t_eat);
		pthread_mutex_lock(&philo->set->death_mutex);
	}
	pthread_mutex_unlock(&philo->set->death_mutex);
	return (RETURN_SUCCESS);
}

// in case of edge case we do a special routine that use a custom
// usleep to avoid getting stuck in such cases

static void	fragmented_usleep_odd(t_philo *philo)
{
	pthread_mutex_lock(&philo->set->death_mutex);
	while (!philo->set->death && philo->meals_eaten != philo->set->max_meal)
	{
		pthread_mutex_unlock(&philo->set->death_mutex);
		routine_take_fork(philo, false);
		routine_take_fork(philo, true);
		print_msg_routine(philo, IS_EATING);
		ft_usleep(philo->set->t_eat, philo->set);
		pthread_mutex_lock(&philo->left->mutex);
		philo->left->available = true;
		pthread_mutex_unlock(&philo->left->mutex);
		pthread_mutex_lock(&philo->right->mutex);
		philo->right->available = true;
		pthread_mutex_unlock(&philo->right->mutex);
		print_msg_routine(philo, IS_SLEEPING);
		ft_usleep(philo->set->t_sleep, philo->set);
		print_msg_routine(philo, IS_THINKING);
		if (!philo->set->nbr_philo_odd)
			ft_usleep(philo->set->t_eat, philo->set);
		pthread_mutex_lock(&philo->set->death_mutex);
	}
	pthread_mutex_unlock(&philo->set->death_mutex);
}

// wait all thread then check if we are dealing with
// an edge case (like if the time to eat is a ridiculous amount)
// in comparison of time to die
// then we +1 our value of full pasta to show that our philo finished to eat
// the right amount of meal

void	*routine_odd(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_threads(philo->set, philo);
	if (philo->set->edge_case)
	{
		print_msg_routine(philo, IS_THINKING);
		ft_usleep(philo->set->t_eat / 6, philo->set);
		fragmented_usleep_odd(philo);
	}
	else if (routine_odd_loop(philo))
	{
		usleep(philo->set->t_die);
		return (EXIT_SUCCESS);
	}
	pthread_mutex_lock(&philo->set->pasta_mutex);
	if (philo->meals_eaten == philo->set->max_meal)
		philo->set->philo_full_pasta = philo->set->philo_full_pasta + 1;
	pthread_mutex_unlock(&philo->set->pasta_mutex);
	return (EXIT_SUCCESS);
}
