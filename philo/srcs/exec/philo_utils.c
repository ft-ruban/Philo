#include "exec.h"
#include <sys/time.h> //getting time of day need it
#include <unistd.h> //usleep

void routine_take_fork(t_philo *philo, bool right)
{
        if(right)
        {
            pthread_mutex_lock(&philo->right->mutex);
            philo->right->available = false;
            print_msg_routine(philo, IS_TAKING_FORK);
        }
        else
        {
            pthread_mutex_lock(&philo->left->mutex);
            philo->left->available = false;
            print_msg_routine(philo, IS_TAKING_FORK);
        }
}
long fill_now()
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return(tv.tv_sec * 1000000 + tv.tv_usec);
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
    {
        gettimeofday(&tv, NULL);
        now = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
        printf("%ld %ld is thinking\n",now / 1000, philo->id);
    }  
    else if (cases == IS_TAKING_FORK && philo->set->death != true)
    {
        gettimeofday(&tv, NULL);
        now = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
        printf("%ld %ld has taken a fork\n",now / 1000, philo->id);
    }
    else if (cases == IS_SLEEPING && philo->set->death != true)
    {
        gettimeofday(&tv, NULL);
        now = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
        printf("%ld %ld is sleeping\n",now / 1000, philo->id);
    }
        
    pthread_mutex_unlock(&philo->set->print_mutex);
}
