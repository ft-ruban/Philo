/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 14:56:59 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/20 15:20:20 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include <unistd.h> //usleep

static void	think_fork_even(t_philo *philo
		,bool *break_loop)
{
	//print_msg_routine(philo, IS_THINKING);
	usleep(500); //check si avec odd ca marche
	routine_take_fork(philo, false);
	if (philo->right->id == philo->left->id)
	{
		*break_loop = true;
		return ;
	}
	routine_take_fork(philo, true);
	print_msg_routine(philo, IS_EATING);
	ft_usleep(philo->set->t_eat,philo->set);
	pthread_mutex_lock(&philo->left->mutex);
	philo->left->available = true;
	pthread_mutex_unlock(&philo->left->mutex);
	pthread_mutex_lock(&philo->right->mutex);	
	philo->right->available = true;
	pthread_mutex_unlock(&philo->right->mutex);
}

static void	think_fork_odd(t_philo *philo/*, bool first_iteration*/)
{
	//print_msg_routine(philo, IS_THINKING);
	// if (first_iteration)
	// {
	// 	//usleep(10);
	// 	//*first_iteration = false;
	// 	//ft_usleep(philo->set->t_eat,philo->set);
	// }
	// //else
	// 	//usleep(10);
	//usleep(250);
	routine_take_fork(philo, true);
	routine_take_fork(philo, false);
	print_msg_routine(philo, IS_EATING);
	ft_usleep(philo->set->t_eat,philo->set);
	pthread_mutex_lock(&philo->right->mutex);
	philo->right->available = true;
	pthread_mutex_unlock(&philo->right->mutex);
	pthread_mutex_lock(&philo->left->mutex);
	philo->left->available = true;
	pthread_mutex_unlock(&philo->left->mutex);
}

void	*routine_odd(void *arg)
{
	t_philo	*philo;
	bool	first_iteration;

	philo = (t_philo *)arg;
	print_msg_routine(philo, IS_THINKING);
	usleep(philo->set->t_eat / 2);
	first_iteration = true;
	pthread_mutex_lock(&philo->set->death_mutex);
	while (!philo->set->death && philo->meals_eaten != philo->set->max_meal)
	{
		pthread_mutex_unlock(&philo->set->death_mutex);
		think_fork_odd(philo/*, first_iteration*/);
		print_msg_routine(philo, IS_SLEEPING);
		ft_usleep(philo->set->t_sleep,philo->set);
		print_msg_routine(philo, IS_THINKING);
		if(!philo->set->nbr_philo_odd)
			usleep(philo->set->t_eat / 2);
		first_iteration = false;
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
	bool	break_loop;
	bool	first_iteration;

	first_iteration = true;
	philo = (t_philo *)arg;
	print_msg_routine(philo, IS_THINKING);
	usleep(philo->set->t_eat / 4);
	break_loop = false;
	pthread_mutex_lock(&philo->set->death_mutex);
	while (!philo->set->death && philo->meals_eaten != philo->set->max_meal)
	{
		pthread_mutex_unlock(&philo->set->death_mutex);
		think_fork_even(philo, &break_loop);
		if (break_loop)
			break ;
		print_msg_routine(philo, IS_SLEEPING);
		ft_usleep(philo->set->t_sleep,philo->set);
		print_msg_routine(philo, IS_THINKING);
		if (!philo->set->nbr_philo_odd)
			usleep(philo->set->t_eat / 2);
		first_iteration = false;
		pthread_mutex_lock(&philo->set->death_mutex);
	}
	if(!break_loop)
		pthread_mutex_unlock(&philo->set->death_mutex);
	pthread_mutex_lock(&philo->set->pasta_mutex);
	if (philo->meals_eaten == philo->set->max_meal)
		philo->set->philo_full_pasta = philo->set->philo_full_pasta + 1;
	pthread_mutex_unlock(&philo->set->pasta_mutex);
	return (0);
}
