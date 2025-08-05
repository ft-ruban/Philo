#include "exec.h"
#include <sys/time.h> //getting time of day need it

void create_thread(t_philo *philo)
{
    while(philo)
    {
        if(pthread_create(&philo->thread_id, NULL, &routine_even, philo) != 0)
            return;
        philo = philo->next;
        //usleep(5000);
        if(philo)
        {
            if(pthread_create(&philo->thread_id, NULL, &routine_odd, philo) != 0)
                return;
            philo = philo->next;
            //usleep(5000);
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

    gettimeofday(&tv, NULL);
    philo->set->time_passed = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
    pthread_mutex_lock(&philo->set->print_mutex);
    if(cases == IS_THINKING)
        printf("%ld %ld is thinking\n",philo->set->time_passed / 1000, philo->id);
    else if (cases == IS_TAKING_FORK)
        printf("%ld %ld has taken a fork\n",philo->set->time_passed / 1000, philo->id);
    else if (cases == IS_EATING)
        printf("%ld %ld is eating\n",philo->set->time_passed / 1000, philo->id);
    else if (cases == IS_SLEEPING)
        printf("%ld %ld is sleeping\n",philo->set->time_passed / 1000, philo->id);
    pthread_mutex_unlock(&philo->set->print_mutex);
}
