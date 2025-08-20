/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:00:47 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/18 15:42:21 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include "exec.h"
#include "parsing.h"
#include <stdlib.h> //malloc
#include <unistd.h> //write

//TODL malloc secure until line 36

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
	{
		write(2, "main:30 : Malloc error\n", 24);
		return (EXIT_FAILURE);
	}
	if (parsing(argc, argv, set))
		return (EXIT_FAILURE);
	philo = malloc(sizeof(t_philo) * set->nbr_philo);
	forks = malloc(sizeof(t_forks) * set->nbr_philo);
	if (!philo || !forks)
		return (free_structs(set, philo, forks, EXIT_FAILURE)); //TODO error msg about malloc errors
	if(setup_philo_forks_struct(set, philo, forks))
		return(free_structs(set, philo, forks, EXIT_FAILURE));
	philosopher(set, philo);
	destroy_mutex(set, philo, forks, set->nbr_philo - 1); //devrait etre good enough
	//destroy nos mutex?
	free_structs(set, philo, forks, EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
