#include "../include/philo.h"
#include <sys/time.h> //gettingtimeofdayneedit
#include <time.h> //time_t type need it
#include <stdlib.h> //malloc
#include "parsing.h"
#include <pthread.h> //phtread_t
#include <unistd.h> //usleep

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

static void philosopher(t_settings *set, t_philo *philo, t_forks *forks)
{
    time_t to_sub_unit;
    time_t to_sub_micro;
    struct timeval tv;
    long result;

    gettimeofday(&tv, NULL);
    to_sub_unit = tv.tv_sec;
    to_sub_micro = tv.tv_usec;
    printf("Microseconds part: %ld\n", tv.tv_usec);
    usleep(5005000);
    gettimeofday(&tv, NULL);
    printf("time : %ld + %ld\n", tv.tv_sec - to_sub_unit, tv.tv_usec - to_sub_micro);
    result = (tv.tv_sec - to_sub_unit) * 1000000 + (tv.tv_usec - to_sub_micro);
    printf("result = : %ld \n",result / 1000);
    //printf("milisecond : %ld \n", (tv.tv_sec - to_sub_unit) * 1000  + (tv.tv_usec - to_sub_micro) / 1000);
    set = NULL;
    philo = NULL;
    forks = NULL;
    printf("nice");
    // thread_maker(set, philo, forks);
    //while(!set->death)
}

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
