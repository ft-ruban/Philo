/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevoude <ldevoude@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:00:47 by ldevoude          #+#    #+#             */
/*   Updated: 2025/08/08 15:01:19 by ldevoude         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
 
#include <stdlib.h> //malloc
#include "parsing.h"
#include "exec.h"
//#include <pthread.h> //phtread_t
#include <unistd.h> //write

int main(int argc, char *argv[])
{
    t_settings *set;
    t_philo    *philo;
    t_forks    *forks;

    set = malloc(sizeof (t_settings));
    if(!set)
    {
        write(2,"main:47 : Malloc error\n", 24);
        return (EXIT_FAILURE);
    }
    if (parsing(argc, argv, set))
    {
        printf("PARSING ERROR\n"); //TORM
        return(EXIT_FAILURE);
    }
    philo = malloc(sizeof (t_philo) * set->nbr_philo);
    forks = malloc(sizeof (t_forks) * set->nbr_philo);
    if(!philo || !forks)
        return free_structs(set, philo, forks, EXIT_FAILURE);
    create_philo(set, philo, forks);
    print_id_related_forks(philo, forks); //TORM
    philosopher(set, philo, forks);
    free_structs(set, philo, forks, EXIT_SUCCESS);
    return (EXIT_SUCCESS);
}
