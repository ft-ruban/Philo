#include "../include/philo.h"
#include <sys/time.h> //gettingtimeofdayneedit
#include <time.h> //time_t type need it
#include <stdlib.h> //malloc
#include "parsing.h"

//pot soucis si que un philo 


int main(int argc, char *argv[])
{
    t_settings *set;
    t_philo    *philo;
    t_forks    *forks;

    set = malloc(sizeof (t_settings));
    if(!set)
        return (EXIT_FAILURE);
    if (parsing(argc, argv, set))
    {
        printf("PARSING ERROR\n"); //TORM
        return(RETURN_ERROR);
    }
    else
        printf("PARSING SUCCESS\n"); //TORM
    philo = malloc(sizeof (t_philo) * set->nbr_philo);//PROT
    forks = malloc(sizeof (t_forks) * set->nbr_philo);//PROT
    if(!philo || !forks)
        return free_structs(set, philo, forks, EXIT_FAILURE);
    create_philo(set->nbr_philo, philo, forks);
    print_id_related_forks(philo, forks);
    free_structs(set, philo, forks, EXIT_SUCCESS);
    return (0);
}
