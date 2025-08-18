/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:23:13 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/18 11:34:13 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include "stdlib.h"

static int	destroy_mutex(t_settings *settings, t_philo *philo, t_forks *forks,
		long i)
{
	if (settings->bool_print_mutex == true)
		pthread_mutex_destroy(&settings->print_mutex);
	if (settings->bool_death_mutex == true)
		pthread_mutex_destroy(&settings->death_mutex);
	while (i >= 0)
	{
		if (forks[i].bool_mutex == true)
			pthread_mutex_destroy(&forks[i].mutex);
		if (philo[i].bool_alive_mutex == true)
			pthread_mutex_destroy(&philo[i].t_alive_mutex);
		i--;
	}
	return (RETURN_FAILURE);
}

static void	setup_bools(t_philo *philo, t_forks *forks, t_settings *set)
{
	long	i;

	i = 0;
	while (i < set->nbr_philo)
	{
		forks[i].available = true;
		forks[i].bool_mutex = false;
		philo[i].bool_alive_mutex = false;
		i++;
	}
	set->bool_print_mutex = false;
	set->bool_death_mutex = false;
}

static int	setup_last_node(t_settings *settings, t_philo *philo,
		t_forks *forks, long i)
{
	philo[i].id = i + 1;
	philo[i].meals_eaten = 0;
	philo[i].left = &forks[i];
	philo[i].right = &forks[0];
	philo[i].next = NULL;
	philo[i].set = settings;
	forks[i].id = i + 1;
	if (pthread_mutex_init(&forks[i].mutex, NULL))
		return (destroy_mutex(settings, philo, forks, i));
	forks[i].bool_mutex = true;
	if (pthread_mutex_init(&philo[i].t_alive_mutex, NULL))
		return (destroy_mutex(settings, philo, forks, i));
	philo[i].bool_alive_mutex = true;
	forks[i].next = NULL;
	if (pthread_mutex_init(&philo[i].set->print_mutex, NULL))
		return (destroy_mutex(settings, philo, forks, i));
	philo[i].set->bool_print_mutex = true;
	if (pthread_mutex_init(&philo[i].set->death_mutex, NULL))
		return (destroy_mutex(settings, philo, forks, i));
	philo[i].set->bool_death_mutex = true;
	return (RETURN_SUCCESS);
}

int	setup_philo_forks_struct(t_settings *settings, t_philo *philo,
		t_forks *forks)
{
	long	i;

	i = 0;
	setup_bools(philo, forks, settings);
	while (i < settings->nbr_philo - 1)
	{
		philo[i].id = i + 1;
		philo[i].meals_eaten = 0;
		philo[i].left = &forks[i];
		philo[i].right = &forks[i + 1];
		philo[i].next = &philo[i + 1];
		philo[i].set = settings;
		forks[i].id = i + 1;
		if (pthread_mutex_init(&forks[i].mutex, NULL))
			return (destroy_mutex(settings, philo, forks, i));
		forks[i].bool_mutex = true;
		if (pthread_mutex_init(&philo[i].t_alive_mutex, NULL))
			return (destroy_mutex(settings, philo, forks, i));
		philo[i].bool_alive_mutex = true;
		forks[i].next = &forks[i + 1];
		i++;
	}
	return (setup_last_node(settings, philo, forks, i));
}

int	free_structs(t_settings *set, t_philo *philo, t_forks *forks,
		int return_value)
{
	if (set)
		free(set);
	if (philo)
		free(philo);
	if (forks)
		free(forks);
	return (return_value);
}
