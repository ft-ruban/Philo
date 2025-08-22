/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 14:56:59 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/22 10:58:43 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h> //gettingtimeofday
#include "exec.h"
#include <unistd.h> //usleep

static void	fork_eat_even(t_philo *philo
		,bool *break_loop)
{
	routine_take_fork(philo, false);
	if (philo->right->id == philo->left->id)
	{
		*break_loop = true;
		return ;
	}
	routine_take_fork(philo, true);
	print_msg_routine(philo, IS_EATING);
	usleep(philo->set->t_eat);
	//ft_usleep(philo->set->t_eat,philo->set);
	pthread_mutex_lock(&philo->left->mutex);
	philo->left->available = true;
	pthread_mutex_unlock(&philo->left->mutex);
	pthread_mutex_lock(&philo->right->mutex);	
	philo->right->available = true;
	pthread_mutex_unlock(&philo->right->mutex);
}

static void	fork_eat_odd(t_philo *philo)
{
	routine_take_fork(philo, true);
	routine_take_fork(philo, false);
	print_msg_routine(philo, IS_EATING);
	usleep(philo->set->t_eat);
	//ft_usleep(philo->set->t_eat,philo->set);
	pthread_mutex_lock(&philo->right->mutex);
	philo->right->available = true;
	pthread_mutex_unlock(&philo->right->mutex);
	pthread_mutex_lock(&philo->left->mutex);
	philo->left->available = true;
	pthread_mutex_unlock(&philo->left->mutex);
}

static long	fill_now_variable(long *now)
{
	struct timeval	tv;

	if(gettimeofday(&tv, NULL))
		return(RETURN_FAILURE);
	*now = (tv.tv_sec * 1000000 + tv.tv_usec);
	return (RETURN_SUCCESS);
}

static int wait_all_threads(t_settings *set, t_philo *philo)
{
	long now;

	now = 0;
	pthread_mutex_lock(&set->print_mutex);
	while(!set->start)
	{
		pthread_mutex_unlock(&set->print_mutex);
		usleep(1);
		pthread_mutex_lock(&set->print_mutex);
	}
	pthread_mutex_unlock(&set->print_mutex);
	if(fill_now_variable(&now))
		return(RETURN_FAILURE);
	pthread_mutex_lock(&philo->t_alive_mutex);
	philo->t_alive = now;
	pthread_mutex_unlock(&philo->t_alive_mutex);
	return(RETURN_SUCCESS);
}

void	*routine_odd(void *arg)
{
	t_philo	*philo;
	bool	first_iteration;

	philo = (t_philo *)arg;
	wait_all_threads(philo->set, philo);
	print_msg_routine(philo, IS_THINKING);
	if(philo->set->nbr_philo_odd)
		usleep(philo->set->t_eat / 2);
	first_iteration = true;
	pthread_mutex_lock(&philo->set->death_mutex);
	while (!philo->set->death && philo->meals_eaten != philo->set->max_meal)
	{
		pthread_mutex_unlock(&philo->set->death_mutex);
		fork_eat_odd(philo/*, first_iteration*/);
		print_msg_routine(philo, IS_SLEEPING);
		usleep(philo->set->t_sleep);
		//ft_usleep(philo->set->t_sleep,philo->set);
		print_msg_routine(philo, IS_THINKING);
		if(!philo->set->nbr_philo_odd)
			usleep(philo->set->t_eat);
		// else
		// 	usleep(100);
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

	philo = (t_philo *)arg;
	wait_all_threads(philo->set, philo);
	print_msg_routine(philo, IS_THINKING);
	usleep(philo->set->t_eat / 6);
	break_loop = false;
	pthread_mutex_lock(&philo->set->death_mutex);
	while (!philo->set->death && philo->meals_eaten != philo->set->max_meal)
	{
		pthread_mutex_unlock(&philo->set->death_mutex);
		fork_eat_even(philo, &break_loop);
		if (break_loop)
			break ;
		print_msg_routine(philo, IS_SLEEPING);
		usleep(philo->set->t_sleep);
		//ft_usleep(philo->set->t_sleep,philo->set);
		print_msg_routine(philo, IS_THINKING);
		if (!philo->set->nbr_philo_odd)
			usleep(philo->set->t_eat);
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
