
#include "../include/philo.h"
//#include "stdlib.h"


//setup the last node for the last philo and fork
//including mutex of our set struct

static int	setup_last_node(t_settings *settings, t_philo *philo,
		t_forks *forks, long i)
{
	philo[i].id = i + 1;
	philo[i].meals_eaten = 0;
	philo[i].left = &forks[i];
	philo[i].right = &forks[0];
	philo[i].next = NULL;
	philo[i].set = settings;
	forks[i].id = i + 1;
	if (pthread_mutex_init(&forks[i].mutex, NULL))
		return (destroy_mutex_fail(settings, philo, forks, i));
	forks[i].bool_mutex = true;
	if (pthread_mutex_init(&philo[i].t_alive_mutex, NULL))
		return (destroy_mutex_fail(settings, philo, forks, i));
	philo[i].bool_alive_mutex = true;
	forks[i].next = NULL;
	if (pthread_mutex_init(&philo[i].set->print_mutex, NULL))
		return (destroy_mutex_fail(settings, philo, forks, i));
	philo[i].set->bool_print_mutex = true;
	if (pthread_mutex_init(&philo[i].set->death_mutex, NULL))
		return (destroy_mutex_fail(settings, philo, forks, i));
	philo[i].set->bool_death_mutex = true;
	if (pthread_mutex_init(&philo->set->pasta_mutex, NULL))
		return (destroy_mutex_fail(settings, philo, forks, i));
	philo[i].set->bool_pasta_mutex = true;
	return (RETURN_SUCCESS);
}

//fill node nbr i with the right values
//also the mutex are protected in case of failure

static int fill_node_i(t_settings *settings, t_philo *philo,
		t_forks *forks, long i)
{
        philo[i].id = i + 1;
		philo[i].meals_eaten = 0;
		philo[i].left = &forks[i];
		philo[i].right = &forks[i + 1];
		philo[i].next = &philo[i + 1];
		philo[i].set = settings;
		forks[i].id = i + 1;
		if (pthread_mutex_init(&forks[i].mutex, NULL))
			return (RETURN_FAILURE);
		forks[i].bool_mutex = true;
		if (pthread_mutex_init(&philo[i].t_alive_mutex, NULL))
			return (RETURN_FAILURE);
		philo[i].bool_alive_mutex = true;
		forks[i].next = &forks[i + 1];
        return(RETURN_SUCCESS);
}

//setup all of our bool value in forks philo and set

static void	setup_bools(t_philo *philo, t_forks *forks, t_settings *set)
{
	long	i;

	i = 0;
	while (i < set->nbr_philo)
	{
		forks[i].available = true;
		forks[i].bool_mutex = false;
		philo[i].bool_alive_mutex = false;
		i++;
	}
	set->bool_print_mutex = false;
	set->bool_death_mutex = false;
}

//here we setup the philo and fork structure
//by setting up all of our bools in the right way
//then we get into a loop that would fill each node
//to finish on the setup of our last node

int	setup_philo_forks_struct(t_settings *settings, t_philo *philo,
		t_forks *forks)
{
	long	i;

	i = 0;
	setup_bools(philo, forks, settings);
	while (i < settings->nbr_philo - 1)
	{
		if(fill_node_i(settings, philo, forks, i))
            return (destroy_mutex_fail(settings, philo, forks, i));
		i++;
	}
	return (setup_last_node(settings, philo, forks, i));
}
