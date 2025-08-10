
#include <sys/time.h> //gettingtimeofdayneedit
#include "exec.h"
#include <unistd.h> //usleep

static int create_thread(t_philo *philo, t_philo *tmp, long now, bool even)
{
    tmp = philo;
    while(philo)
    {
        now = fill_now();
        pthread_mutex_lock(&philo->t_alive_mutex);
        philo->t_alive = now;
        pthread_mutex_unlock(&philo->t_alive_mutex);
        if(even)
        {
            if(pthread_create(&philo->thread_id, NULL, &routine_even, philo) != 0)
                return(RETURN_FAILURE);
            even = false;
        }
        else
        {
            if (pthread_create(&philo->thread_id, NULL, &routine_odd, philo) != 0)
                return(RETURN_FAILURE);
            even = true;
        }
        philo = philo->next;
    }
    if(pthread_create(&tmp->set->monitor_thread_id, NULL, &philo_monitor, tmp) != 0)
        return(RETURN_FAILURE);
    return(RETURN_SUCCESS);
}

static int join_thread(t_philo *philo)
{

    if(pthread_join(philo->set->monitor_thread_id, NULL))
        return(RETURN_FAILURE); //protect
    while(philo)
    {
        if(pthread_join(philo->thread_id, NULL))
            return(RETURN_FAILURE); //protect?
        philo = philo->next;
    }
    return(RETURN_SUCCESS);
}

void philosopher(t_settings *set, t_philo *philo, t_forks *forks)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    set->subunit = tv.tv_sec;
    set->subusec = tv.tv_usec;
    gettimeofday(&tv, NULL);
    set->time_passed = (tv.tv_sec -  set->subunit) * 1000000 + (tv.tv_usec - set->subusec);
    create_thread(philo, NULL, 0, true); //TOPROTECT
        //retour d'erreur
    join_thread(philo);
    forks = NULL;
    //retour positif
}