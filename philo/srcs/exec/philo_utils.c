#include "exec.h"
#include <sys/time.h> //getting time of day need it

void create_thread(t_philo *philo, t_settings *set)
{
    struct timeval tv;
    t_philo *tmp;

    tmp = philo;
    while(philo)
    {
        gettimeofday(&tv, NULL);
        pthread_mutex_lock(&philo->t_alive_mutex);
        philo->t_alive = tv.tv_sec * 1000000 + tv.tv_usec;
        pthread_mutex_unlock(&philo->t_alive_mutex);
        if(pthread_create(&philo->thread_id, NULL, &routine_even, philo) != 0)
            return;
        philo = philo->next;
        if(philo)
        {
            gettimeofday(&tv, NULL);
            pthread_mutex_lock(&philo->t_alive_mutex);
            philo->t_alive = tv.tv_sec * 1000000 + tv.tv_usec;
            pthread_mutex_unlock(&philo->t_alive_mutex);
            if(pthread_create(&philo->thread_id, NULL, &routine_odd, philo) != 0)
                return;
            philo = philo->next;
        }
    }
    if(pthread_create(&set->monitor_thread_id, NULL, &philo_monitor, tmp) != 0)
        return;

}
void join_thread(t_philo *philo)
{

    pthread_join(philo->set->monitor_thread_id, NULL);
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
    pthread_mutex_lock(&philo->set->print_mutex);
    philo->set->time_passed = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
    
    if (cases == IS_EATING && philo->set->death != true)
    {
        gettimeofday(&tv, NULL);
        pthread_mutex_lock(&philo->t_alive_mutex);
        philo->t_alive = tv.tv_sec * 1000000 + tv.tv_usec;
        pthread_mutex_unlock(&philo->t_alive_mutex);
        philo->meals_eaten = philo->meals_eaten + 1;
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
