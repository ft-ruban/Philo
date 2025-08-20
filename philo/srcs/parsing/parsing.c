/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 10:50:22 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/18 15:27:23 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"

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

static int	ft_isnum(int c)
{
	if (c >= '0' && c <= '9')
		return (RETURN_SUCCESS);
	else
		return (RETURN_ERROR);
}

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
		return (error_msg(set, INVALID_ARGC));
	if(check_if_num_val(argv))
		return (error_msg(set, INVALID_NUM_VALUE));
	return_value = convert_argv_to_struct_utils(argv, set);
	if (return_value)
		return (error_msg(set, return_value));
	return (RETURN_SUCCESS);
}
