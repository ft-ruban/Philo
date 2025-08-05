#include "../include/philo.h"
 
#include <time.h> //time_t type need it
#include <stdlib.h> //malloc
#include "parsing.h"
#include "exec.h"
#include <pthread.h> //phtread_t

//pot soucis si que un philo 

// void* routine()
// {
//     while(//notdead)
//         //think
//         //check if left fork
//         //pickupleftfork
//         //check if right fork
//         //pickuprightfork
//         //put_down_left_fork();
//         //put_down_right_fork();
//         //sleep();
//         //eat
// }

// static int thread_maker(t_settings *set, t_philo *philo, t_forks *forks)
// {
//     pthread_t th[set->nbr_philo];
//     int i = 0;

//     while(i < set->nbr_philo)
//     {
//         if(pthread_create(th + i,  NULL, &routine, philo) != 0)
// 		    return(1);
// 	    printf("thread %d has started\n", i);
//     }
// }

//TODO renseigner 


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
    philo = malloc(sizeof (t_philo) * set->nbr_philo);
    forks = malloc(sizeof (t_forks) * set->nbr_philo);
    if(!philo || !forks)
        return free_structs(set, philo, forks, EXIT_FAILURE);
    create_philo(set, philo, forks);
    print_id_related_forks(philo, forks); //TORM
    philosopher(set, philo, forks);
    free_structs(set, philo, forks, EXIT_SUCCESS);
    return (0);
}
