/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:00:47 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/22 10:27:40 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

//#include "../include/philo.h"
#include "exec.h"
#include "parsing.h"
//#include <stdlib.h> //malloc
#include <unistd.h> //write

//TODO dans setup peut etre faire du long un size_t (et donc aussi nbr philo) c plus propre

//at first we malloc our set struct then check the arguments entered 
//in our parsing we also fill our set struct in the parsing -
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
