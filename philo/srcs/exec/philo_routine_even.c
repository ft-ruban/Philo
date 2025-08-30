/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_even.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 14:33:57 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/30 14:33:57 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "exec.h"

static void fragmented_usleep_even(t_philo *philo)
{
	print_msg_routine(philo, IS_THINKING);
	if(philo->set->nbr_philo_odd)
		ft_usleep(philo->set->t_eat / 2, philo->set);
	pthread_mutex_lock(&philo->set->death_mutex);
	while (!philo->set->death && philo->meals_eaten != philo->set->max_meal)
	{
		pthread_mutex_unlock(&philo->set->death_mutex);
		routine_take_fork(philo, true);
		routine_take_fork(philo, false);
		print_msg_routine(philo, IS_EATING);
		ft_usleep(philo->set->t_eat, philo->set);
		pthread_mutex_lock(&philo->right->mutex);
		philo->right->available = true;
		pthread_mutex_unlock(&philo->right->mutex);
		pthread_mutex_lock(&philo->left->mutex);
		philo->left->available = true;
		pthread_mutex_unlock(&philo->left->mutex);
		print_msg_routine(philo, IS_SLEEPING);
		ft_usleep(philo->set->t_sleep, philo->set);
		print_msg_routine(philo, IS_THINKING);
		if(!philo->set->nbr_philo_odd)
			ft_usleep(philo->set->t_eat, philo->set);
		pthread_mutex_lock(&philo->set->death_mutex);
	}
	pthread_mutex_unlock(&philo->set->death_mutex);
}

//here even philo take their right forks then left
//eat, and make there forks available once more

static void	fork_eat_even(t_philo *philo)
{
	routine_take_fork(philo, true);
	routine_take_fork(philo, false);
	print_msg_routine(philo, IS_EATING);
	usleep(philo->set->t_eat);
	pthread_mutex_lock(&philo->right->mutex);
	philo->right->available = true;
	pthread_mutex_unlock(&philo->right->mutex);
	pthread_mutex_lock(&philo->left->mutex);
	philo->left->available = true;
	pthread_mutex_unlock(&philo->left->mutex);
}

static void    routine_even_loop(t_philo *philo)
{
	print_msg_routine(philo, IS_THINKING);
	if(philo->set->nbr_philo_odd)
		usleep(philo->set->t_eat / 2);
    pthread_mutex_lock(&philo->set->death_mutex);
	while (!philo->set->death && philo->meals_eaten != philo->set->max_meal)
	{
		pthread_mutex_unlock(&philo->set->death_mutex);
		fork_eat_even(philo/*, first_iteration*/);
		print_msg_routine(philo, IS_SLEEPING);
		usleep(philo->set->t_sleep);
		print_msg_routine(philo, IS_THINKING);
		if(!philo->set->nbr_philo_odd)
			usleep(philo->set->t_eat);
		pthread_mutex_lock(&philo->set->death_mutex);
	}
	pthread_mutex_unlock(&philo->set->death_mutex);
}

//this is where our even philo's threads would start.
//at first they wait in wait all thread until all threads
//are created. then it start to think, a usleep of t_eat / 2
//is required WHEN the total nbr of philo is even
//for opti purpose in such case
//then we get into our while loop that represent the general routine
//of our even_nbr philo's threads

void	*routine_even(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_threads(philo->set, philo);
	if (philo->set->edge_case)
		fragmented_usleep_even(philo);
	else
		routine_even_loop(philo);
	pthread_mutex_lock(&philo->set->pasta_mutex);
	if (philo->meals_eaten == philo->set->max_meal)
		philo->set->philo_full_pasta = philo->set->philo_full_pasta + 1;
	pthread_mutex_unlock(&philo->set->pasta_mutex);
	return (0);
}
