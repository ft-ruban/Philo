/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:00:47 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/21 16:16:15 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

//#include "../include/philo.h"
#include "exec.h"
#include "parsing.h"
#include <stdlib.h> //malloc
#include <unistd.h> //write

//Function that will destroy all our mutex in case of a successful parsing
//similar to destroy_mutex_failure, difference here is 
//the importance of return value
static int	destroy_mutex_success(t_settings *settings, t_philo *philo, t_forks *forks,
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

static int free_write_then_exit_program(int error_type, t_settings *set, t_philo *philo, t_forks *forks)
{
	free_structs(set, philo, forks, EXIT_FAILURE);
	if (error_type == MALLOC_SET_ERROR)
		write(2, "main:36 : Malloc error for essential structure\n", 24);
	else if (error_type == PARSING_ERROR)
		return(2);
	else if (error_type == MALLOC_PHILO_OR_FORK_ERROR)
		write(2, "main:45 : Malloc error for essential structures\n",49);
	else if (error_type == SETUP_STRUCT_ERROR)
		write(2, "main:50 : error during init of mutex\n", 38);
	else if (error_type == EXEC_ERROR)
	{
		write(2, "main:xx : error during the execution of philosopher", 54);
		destroy_mutex_fail(set, philo, forks, set->nbr_philo - 1);
	}
	else if (error_type == DESTROY_MUTEX_ERROR)
		write(2, "main:xx : error during destruction of mutex", 46);
	return(error_type);
}

//at first we malloc our set struct then check the arguments entered 
//in our parsing we also fill our set struct in the parsing
//once parsing successful we setup philo struct and forks struct
//once done we can start our simulation once done we destroy 
//mutex, free our struct to exit properly

int	main(int argc, char *argv[])
{
	t_settings	*set;
	t_philo		*philo;
	t_forks		*forks;

	set = malloc(sizeof(t_settings));
	if (!set)
		return(free_write_then_exit_program(MALLOC_SET_ERROR, NULL, NULL, NULL));
	if (parsing(argc, argv, set))
		return(free_write_then_exit_program(PARSING_ERROR, set, NULL, NULL));
	philo = malloc(sizeof(t_philo) * set->nbr_philo);
	forks = malloc(sizeof(t_forks) * set->nbr_philo);
	if (!philo || !forks)
		return(free_write_then_exit_program(MALLOC_PHILO_OR_FORK_ERROR, set, philo, NULL));
	if(setup_philo_forks_struct(set, philo, forks))
		return(free_write_then_exit_program(SETUP_STRUCT_ERROR, set, philo, forks));
	if(prepare_creation_thread(philo, philo, true))
		return(free_write_then_exit_program(EXEC_ERROR, set, philo, forks));
	if(destroy_mutex_success(set, philo, forks, set->nbr_philo - 1))
		return(free_write_then_exit_program(DESTROY_MUTEX_ERROR, set, philo, forks));
	free_structs(set, philo, forks, EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
