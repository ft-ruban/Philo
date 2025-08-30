/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_error_msg.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 10:34:54 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/30 15:31:48 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parsing.h"
#include <unistd.h>

//write in fd 2 the right error msg

static void	msg_sleep_maxmeal_sleep_plus_eat(int return_value)
{
	if (return_value == INVALID_SLEEP)
	{
		write(2, "philo: INVALID_SLEEP: invalid input enter ", 43);
		write(2, "a value between 0 and 9223372036854775 ms\n", 43);
	}
	else if (return_value == INVALID_MAXMEAL)
	{
		write(2, "philo: INVALID_MAXMEAL: invalid input enter ", 45);
		write(2, "a value between 0 and 99999999999999999\n", 41);
	}
	else if (return_value == INVALID_SLEEP_PLUS_EAT)
	{
		write(2, "philo: INVALID_SLEEP_PLUS_EAT: the sum of those", 48);
		write(2, " parameters overflow 9223372036854775 ms\n", 42);
	}
	else if (return_value == INVALID_NUM_VALUE)
	{
		write(2, "philo: INVALID_NUM_VALUE: invalid input", 40);
		write(2, " enter only numerical values (0-9)\n", 36);
	}
}

//write in fd 2 the right error msg

static void	msg_argc_philo_die_eat(int return_value)
{
	if (return_value == INVALID_ARGC)
		write(2, "philo: INVALID_ARGC: only support 4 or 5 parameters\n", 53);
	else if (return_value == INVALID_PHILO)
	{
		write(2, "philo: INVALID_PHILO: ", 23);
		write(2, "invalid input enter a value between 1 and 250\n", 47);
	}
	else if (return_value == INVALID_DIE)
	{
		write(2, "philo: INVALID_DIE: invalid input ", 35);
		write(2, "enter a value between 0 and 9223372036854775 ms\n", 49);
	}
	else if (return_value == INVALID_EAT)
	{
		write(2, "philo: INVALID_EAT: invalid input enter", 40);
		write(2, " a value between 0 and 9223372036854775 ms\n", 44);
	}
}

//here we redirect the program at the right place to write
//the right error msg 

int	error_msg(int return_value)
{
	if (return_value >= 2 && return_value <= 5)
		msg_argc_philo_die_eat(return_value);
	else
		msg_sleep_maxmeal_sleep_plus_eat(return_value);
	return (RETURN_ERROR);
}
