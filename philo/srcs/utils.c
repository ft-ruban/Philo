/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 13:23:13 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/30 14:42:37 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
//#include "stdlib.h"
#include <unistd.h>

//TODO complete all mutex (pastamutex for ex)

int destroy_mutex_nodes(t_philo *philo, t_forks *forks, long i)
{
	while (i >= 0)
	{
		if (forks[i].bool_mutex == true)
		{
			if(pthread_mutex_destroy(&forks[i].mutex))
				return(RETURN_FAILURE);
		}
		if (philo[i].bool_alive_mutex == true)
		{
			if(pthread_mutex_destroy(&philo[i].t_alive_mutex))
				return(RETURN_FAILURE);
		}
		i--;
	}
	return(RETURN_SUCCESS);
}

int	destroy_mutex_fail(t_settings *settings, t_philo *philo, t_forks *forks,
		long i)
{
	if (settings->bool_print_mutex == true)
	{
		if(pthread_mutex_destroy(&settings->print_mutex))
			return (RETURN_FAILURE);
	}
	if (settings->bool_death_mutex == true)
	{
		if(pthread_mutex_destroy(&settings->death_mutex))
			return (RETURN_FAILURE);
	}
	if (settings->bool_pasta_mutex == true)
	{
		if(pthread_mutex_destroy(&settings->pasta_mutex))
			return(RETURN_FAILURE);
	}
	if(destroy_mutex_nodes(philo, forks, i))
		return(RETURN_FAILURE);
	return (RETURN_FAILURE);
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

//Function that will destroy all our mutex in case of a successful parsing
//similar to destroy_mutex_failure, difference here is 
//the importance of return value
int	destroy_mutex_success(t_settings *settings, t_philo *philo, t_forks *forks,
		long i)
{
	if (settings->bool_print_mutex == true)
	{
		if(pthread_mutex_destroy(&settings->print_mutex))
			return (RETURN_FAILURE);
	}
	if (settings->bool_death_mutex == true)
	{
		if(pthread_mutex_destroy(&settings->death_mutex))
			return (RETURN_FAILURE);
	}
	if (settings->bool_pasta_mutex == true)
	{
		if(pthread_mutex_destroy(&settings->pasta_mutex))
			return(RETURN_FAILURE);
	}
	if(destroy_mutex_nodes(philo, forks, i))
		return(RETURN_FAILURE);
	return (RETURN_SUCCESS);
}


//would free the necessary structs then write the associated err
//msg before returning the right type ending the program
//bcs this should only be called in the main function

int free_write_then_exit_program(int error_type, t_settings *set, t_philo *philo, t_forks *forks)
{
	if (error_type == MALLOC_SET_ERROR)
		write(2, "main:30 : Malloc error for essential structure\n", 48);
	else if (error_type == PARSING_ERROR)
	{
		free(set);
		return(2);
	}
	else if (error_type == MALLOC_PHILO_OR_FORK_ERROR)
		write(2, "main:36 : Malloc error for essential structures\n",49);
	else if (error_type == SETUP_STRUCT_ERROR)
		write(2, "main:50 : error during init of mutex\n", 38);
	else if (error_type == EXEC_ERROR)
	{
		write(2, "main:xx : error during the execution of philosopher", 52);
		destroy_mutex_fail(set, philo, forks, set->nbr_philo - 1);
	}
	else if (error_type == DESTROY_MUTEX_ERROR)
		write(2, "main:xx : error during destruction of mutex", 44);
	free_structs(set, philo, forks, EXIT_FAILURE);
	return(error_type);
}
