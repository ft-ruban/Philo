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

void* routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    struct timeval tv;

    usleep(5006000);
    gettimeofday(&tv, NULL);
    philo->set->time_passed = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
    printf("[%ld] %ld has taken a fork\n",philo->set->time_passed / 1000, philo->id);
    gettimeofday(&tv, NULL);
    philo->set->time_passed = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
    printf("[%ld] %ld is eating\n",philo->set->time_passed / 1000, philo->id);
    gettimeofday(&tv, NULL);
    philo->set->time_passed = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
    printf("[%ld] %ld is sleeping\n",philo->set->time_passed / 1000, philo->id);
    gettimeofday(&tv, NULL);
    philo->set->time_passed = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
    printf("[%ld] %ld is thinking\n",philo->set->time_passed / 1000, philo->id);
    gettimeofday(&tv, NULL);
    philo->set->time_passed = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
    printf("[%ld] %ld died\n",philo->set->time_passed / 1000, philo->id);
    pthread_mutex_lock(&philo->set->print_mutex);
    printf("test? %zu\n", philo->id);
    printf("test\n");
    pthread_mutex_unlock(&philo->set->print_mutex);
    return(0);
}

static void create_thread(t_philo *philo)
{
    while(philo)
    {
        if(pthread_create(&philo->thread_id, NULL, &routine, philo) != 0)
            return;
        philo = philo->next;
    }
}
static void join_thread(t_philo *philo)
{
    while(philo)
    {
        pthread_join(philo->thread_id, NULL); //protect?
        philo = philo->next;
    }
    
}

static void philosopher(t_settings *set, t_philo *philo, t_forks *forks)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    set->subunit = tv.tv_sec;
    set->subusec = tv.tv_usec;
    usleep(5006000);
    gettimeofday(&tv, NULL);
    set->time_passed = (tv.tv_sec -  set->subunit) * 1000000 + (tv.tv_usec - set->subusec);
    printf("result = : %ld \n",set->time_passed / 1000);
    create_thread(philo);
    join_thread(philo);
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
