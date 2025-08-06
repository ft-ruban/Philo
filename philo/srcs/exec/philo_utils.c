#include "exec.h"
#include <sys/time.h> //getting time of day need it

void create_thread(t_philo *philo)
{
    struct timeval tv;
    while(philo)
    {
        gettimeofday(&tv, NULL);
        philo->t_alive = tv.tv_sec * 1000000 + tv.tv_usec;
        if(pthread_create(&philo->thread_id, NULL, &routine_even, philo) != 0)
            return;
        philo = philo->next;
        if(philo && philo->next)
        {
            //printf("ICI\n");//TORM
            gettimeofday(&tv, NULL);
            philo->t_alive = tv.tv_sec * 1000000 + tv.tv_usec;
            if(pthread_create(&philo->thread_id, NULL, &routine_odd, philo) != 0)
                return;
            philo = philo->next;
        }
        
    }
}
void join_thread(t_philo *philo)
{
    while(philo)
    {
        pthread_join(philo->thread_id, NULL); //protect?
        philo = philo->next;
    }
    
}

void print_msg_routine(t_philo *philo, size_t cases)
{
    struct timeval tv;
    pthread_mutex_lock(&philo->set->print_mutex);
    //printf("TIME ALIVE??? : %ld\n", philo->t_alive);
    gettimeofday(&tv, NULL);
    
    //printf("TIME ALIVE SUBBB??? : %ld\n", (tv.tv_sec * 1000000 + tv.tv_usec  - philo->t_alive ) / 1000);
    //printf("philo->set->t_die = %ld\n", philo->set->t_die);
    philo->set->time_passed = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);

    if (((tv.tv_sec * 1000000 + tv.tv_usec - philo->t_alive)>= philo->set->t_die) && philo->set->death != true)
    {
        printf("%ld %ld died\n",philo->set->time_passed / 1000, philo->id);
        philo->set->death = true;
    }
    else if (cases == IS_EATING && philo->set->death != true)
    {
        gettimeofday(&tv, NULL);
        philo->t_alive = tv.tv_sec * 1000000 + tv.tv_usec;
        printf("%ld %ld is eating\n",philo->set->time_passed / 1000, philo->id);
    }
    else if(cases == IS_THINKING && philo->set->death != true)
        printf("%ld %ld is thinking\n",philo->set->time_passed / 1000, philo->id);
    else if (cases == IS_TAKING_FORK && philo->set->death != true)
        printf("%ld %ld has taken a fork\n",philo->set->time_passed / 1000, philo->id);
    else if (cases == IS_SLEEPING && philo->set->death != true)
        printf("%ld %ld is sleeping\n",philo->set->time_passed / 1000, philo->id);
    pthread_mutex_unlock(&philo->set->print_mutex);
}
