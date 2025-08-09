/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:00:47 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/09 13:24:17 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include "exec.h"
#include "parsing.h"
#include <stdlib.h> //malloc
#include <unistd.h> //write

int	main(int argc, char *argv[])
{
	t_settings	*set;
	t_philo		*philo;
	t_forks		*forks;

	set = malloc(sizeof(t_settings)); //safe
	if (!set)
	{
		write(2, "main:47 : Malloc error\n", 24);
		return (EXIT_FAILURE);
	}
	if (parsing(argc, argv, set))
		return (EXIT_FAILURE);
	philo = malloc(sizeof(t_philo) * set->nbr_philo); //safe
	forks = malloc(sizeof(t_forks) * set->nbr_philo); //safe
	if (!philo || !forks)
		return (free_structs(set, philo, forks, EXIT_FAILURE));
	if(setup_philo_forks_struct(set, philo, forks))
		return(free_structs(set, philo, forks, EXIT_FAILURE));
	philosopher(set, philo, forks);
	free_structs(set, philo, forks, EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
