/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:00:47 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/30 14:48:01 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "parsing.h"

// TODO dans setup peut etre faire du long un size_t 
//(et donc aussi nbr philo) c plus propre

// at first we malloc our set struct then check the arguments entered
// in our parsing we also fill our set struct in the parsing -
// once parsing successful we setup philo struct and forks struct
// once done we can start our simulation once done we destroy
// mutex, free our struct to exit properly

int	main(int argc, char *argv[])
{
	t_settings	*set;
	t_philo		*philo;
	t_forks		*forks;

	set = malloc(sizeof(t_settings));
	if (!set)
		return (free_write_then_exit_program(1, NULL, NULL, NULL));
	if (parsing(argc, argv, set))
		return (free_write_then_exit_program(2, set, NULL, NULL));
	philo = malloc(sizeof(t_philo) * set->nbr_philo);
	if (!philo)
		return (free_write_then_exit_program(3, set, philo, NULL));
	forks = malloc(sizeof(t_forks) * set->nbr_philo);
	if (!forks)
		return (free_write_then_exit_program(4, set, philo, NULL));
	if (setup_philo_forks_struct(set, philo, forks))
		return (free_write_then_exit_program(4, set, philo, forks));
	if (prepare_creation_thread(philo, philo, true))
		return (free_write_then_exit_program(5, set, philo, forks));
	if (destroy_mutex_success(set, philo, forks, set->nbr_philo - 1))
		return (free_write_then_exit_program(6, set, philo, forks));
	free_structs(set, philo, forks, EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
