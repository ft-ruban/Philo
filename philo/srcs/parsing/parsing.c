/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 10:50:22 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/30 12:41:37 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

//convert the argv into usable values for our
//set structure (also convert into microsecond most
//values to make them usable by usleep function)

static int	convert_argv_to_struct_utils(char *argv[], t_settings *set)
{
	set->death = false;
	set->nbr_philo = ft_atol(argv[1]);
	if (set->nbr_philo < 1 || set->nbr_philo > 250)
		return (INVALID_PHILO);
	set->t_die = (ft_atol(argv[2]) * 1000);
	if (set->t_die < 0 || ft_strlen(argv[2]) > 16)
		return (INVALID_DIE);
	set->t_eat = (ft_atol(argv[3]) * 1000);
	if (set->t_eat < 0 || ft_strlen(argv[3]) > 16)
		return (INVALID_EAT);
	set->t_sleep = (ft_atol(argv[4]) * 1000);
	if (set->t_sleep < 0 || ft_strlen(argv[4]) > 16)
		return (INVALID_SLEEP);
	if (set->t_sleep + set->t_eat < set->t_sleep)
		return (INVALID_SLEEP_PLUS_EAT);
	set->philo_full_pasta = 0;
	if (argv[5])
	{
		set->max_meal = ft_atol(argv[5]);
		if (set->max_meal < 0 || ft_strlen(argv[5]) > 16) //revoir par rapport a la logique qu'on veut ici
			return (INVALID_MAXMEAL);
	}
	else
		set->max_meal = NO_MEAL;
	return (RETURN_SUCCESS);
}

//here we check if the argument given 
//are numeric values, it check all the chars
//of all arguments and if anything is not char
//that represent a num value (0-9) would return 1
//to make the program leave with the right error msg

static int check_if_num_val(char *argv[])
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while(argv[i])
	{
		while(argv[i][j])
		{
			while(argv[i][j] == ' ')
				j++;
			if(ft_isnum((int)argv[i][j]))
				return(RETURN_ERROR);
			j++;
		}
		if(j == 0)
			return(RETURN_ERROR);
		j = 0;
		i++;
	}
	return(RETURN_SUCCESS);
}

// At first we look at if the user sent the right nbr of arguments
// then we check if the user did enter only numbers and not letters
// then we convert our type char argument into an usable type
// and place them into our set struct
// if there is any error during the process we print the necessary error msg
// before exiting our program.

int	parsing(int argc, char *argv[], t_settings *set)
{
	size_t	return_value;

	if (argc > 6 || argc < 5)
		return (error_msg(INVALID_ARGC));
	if(check_if_num_val(argv))
		return (error_msg(INVALID_NUM_VALUE));
	return_value = convert_argv_to_struct_utils(argv, set);
	if (return_value)
		return (error_msg(return_value));
	if (set->nbr_philo % 2 == 0)
		set->nbr_philo_odd = true;
	else
		set->nbr_philo_odd = false;
	set->start = false;
	if(set->t_die > set->t_eat + set->t_sleep)
		set->edge_case = false;
	else 
		set->edge_case = true;
	// printf("edge = %d\n", set->edge_case);
	// printf("tdie = %ld\n", set->t_die);
	// printf("eat = %ld\n", set->t_eat);
	// printf("sleep = %ld\n", set->t_sleep);
	return (RETURN_SUCCESS);
}
