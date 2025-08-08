#include "exec.h"
#include <sys/time.h> //getting time of day need it
#include <unistd.h> //usleep

void usleep_routine(long time_action, t_philo *philo)
{
    long start;
    long present;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    start = tv.tv_sec * 1000000 + tv.tv_usec;
    while (1)
    {
        gettimeofday(&tv, NULL);
        present = tv.tv_sec * 1000000 + tv.tv_usec;

        pthread_mutex_lock(&philo->set->death_mutex);
        if (philo->set->death == true)
        {
            pthread_mutex_unlock(&philo->set->death_mutex);
            break;
        }
        pthread_mutex_unlock(&philo->set->death_mutex);
        if (present - start >= time_action)
            break;
        usleep(100);
    }
}


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
    time_t now;
    
    gettimeofday(&tv, NULL);
    now = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
    pthread_mutex_lock(&philo->set->print_mutex);
    if (cases == IS_EATING && philo->set->death != true)
    {
        gettimeofday(&tv, NULL);
        pthread_mutex_lock(&philo->t_alive_mutex);
        philo->t_alive = tv.tv_sec * 1000000 + tv.tv_usec;
        pthread_mutex_unlock(&philo->t_alive_mutex);
        philo->meals_eaten = philo->meals_eaten + 1;
        printf("%ld %ld is eating\n",now / 1000, philo->id);
    }
    else if(cases == IS_THINKING && philo->set->death != true)
        printf("%ld %ld is thinking\n",now / 1000, philo->id);
    else if (cases == IS_TAKING_FORK && philo->set->death != true)
        printf("%ld %ld has taken a fork\n",now / 1000, philo->id);
    else if (cases == IS_SLEEPING && philo->set->death != true)
        printf("%ld %ld is sleeping\n",now / 1000, philo->id);
    pthread_mutex_unlock(&philo->set->print_mutex);
}
