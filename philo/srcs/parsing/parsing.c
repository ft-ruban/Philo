
#include "../include/parsing.h"
#include <unistd.h> //write
#include <stdlib.h>

// int convert(char *argv[])
// {

// }
// typedef struct s_settings
// {
//     long nbr_philo;
//     long t_die;
//     long t_eat;
//     long t_sleep;
//     long max_meal;
//     bool limit_meal;
//     bool death;
// } t_settings;

//TODO parse in case of overflow also


static int	convert_argv_to_struct_utils(char *argv[], t_settings *set)
{
	set->death = false;
	set->nbr_philo = ft_atol(argv[1]);
	if (set->nbr_philo < 1 || set->nbr_philo > 200)
		return (INVALID_PHILO);
	set->t_die = (ft_atol(argv[2]) * 1000);
	if (set->t_die < 0)
		return (INVALID_DIE);
	set->t_eat = (ft_atol(argv[3])  * 1000);
	if (set->t_eat < 0)
		return (INVALID_EAT);
	set->t_sleep = (ft_atol(argv[4]) * 1000);
	if (set->t_sleep < 0)
		return (INVALID_SLEEP);
	if (argv[5])
	{
		set->limit_meal = true;
		set->max_meal = ft_atol(argv[5]);
		if (set->max_meal < 0)
			return (INVALID_MAXMEAL);
	}
	else
		set->limit_meal = false;
	return (RETURN_SUCCESS);
}
static int	error_msg(t_settings *set, int return_value)
{
	if (return_value == INVALID_ARGC)
		write(2, "philo: INVALID_ARGC: only support 4 or 5 parameters\n", 53);
	if (return_value == INVALID_PHILO)
		write(2,
			"philo: INVALID_PHILO: invalid input enter a value between 1 and 200\n",
			69);
	if (return_value == INVALID_DIE)
		write(2,
			"philo: INVALID_DIE: invalid input enter a value between 0 and LONG_MAX\n",
			73);
	if (return_value == INVALID_EAT)
		write(2,
			"philo: INVALID_EAT: invalid input enter a value between 0 and LONG_MAX\n",
			72);
	if (return_value == INVALID_SLEEP)
		write(2,
			"philo: INVALID_SLEEP: invalid input enter a value between 0 and LONG_MAX\n",
			74);
	if (return_value == INVALID_MAXMEAL)
		write(2,
			"philo: INVALID_MAXMEAL: invalid input enter a value between 0 and LONG_MAX\n",
			76);
	free(set);
	return (RETURN_ERROR);
}

int	parsing(int argc, char *argv[], t_settings *set)
{
	int	return_value;

	if (argc > 6 || argc < 5)
		return (error_msg(set, INVALID_ARGC));
	return_value = convert_argv_to_struct_utils(argv, set);
	if (return_value)
		return (error_msg(set, return_value));
	return (RETURN_SUCCESS);
}
