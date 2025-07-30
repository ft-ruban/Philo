
#include "../include/philo.h"
#include "stdlib.h"

void create_philo(long nbr_philo, t_philo *philo, t_forks *forks)
{
    long i;

    i = 0;
    while(i < nbr_philo - 1) //faire un tour de plus special car fin
    {
        philo[i].id = i + 1;
        philo[i].meals_eaten = 0;
        philo[i].left = &forks[i];
        philo[i].right = &forks[i + 1];
        philo[i].next = &philo[i + 1];
        forks[i].id = i + 1;
        forks[i].available = true;
        forks[i].next = &forks[i + 1];
        i++;
    }
    philo[i].id = i + 1;
    philo[i].meals_eaten = 0;
    philo[i].left = &forks[i];
    philo[i].right = &forks[0];
    philo[i].next = NULL;//&philo[0];
    forks[i].id = i + 1;
    forks[i].available = true;
    forks[i].next = NULL;//&forks[0];
}
int free_structs(t_settings *set,  t_philo *philo, t_forks *forks, int return_value)
{
    if(set)
        free(set);
    if (philo)
        free(philo);
    if(forks)
        free(forks);
    return(return_value);
}
