
#include "exec.h"
#include <sys/time.h>
#include <unistd.h> //usleep

void* philo_monitor(void *arg)
{
    t_philo *tmp;
    t_philo *start = arg;
    long now;
    struct timeval tv;

    tmp = arg;
    pthread_mutex_lock(&start->set->pasta_mutex); 
    while(!start->set->death && start->set->philo_full_pasta != start->set->nbr_philo)
    {
        pthread_mutex_unlock(&start->set->pasta_mutex); 
        tmp = start;
        gettimeofday(&tv, NULL);
        now = tv.tv_sec * 1000000 + tv.tv_usec;
        while(tmp)
        {
            pthread_mutex_lock(&tmp->t_alive_mutex);
            if (now - tmp->t_alive > tmp->set->t_die && tmp->meals_eaten != tmp->set->max_meal)
            {
                pthread_mutex_unlock(&tmp->t_alive_mutex);
                pthread_mutex_lock(&tmp->set->print_mutex);
                if(!tmp->set->death)
                {
                    tmp->set->time_passed = (tv.tv_sec -  tmp->set->subunit) * 1000000 + (tv.tv_usec - tmp->set->subusec);
                    pthread_mutex_lock(&tmp->set->death_mutex);
                    tmp->set->death = true;
                    pthread_mutex_unlock(&tmp->set->death_mutex);
                    printf("%ld %ld died\n",tmp->set->time_passed / 1000, tmp->id);   
                }
                pthread_mutex_unlock(&tmp->set->print_mutex);
                return (NULL);
            }
            pthread_mutex_unlock(&tmp->t_alive_mutex); //TORM?
            tmp = tmp->next;
        }
        usleep(1000);
        pthread_mutex_lock(&start->set->pasta_mutex); 
    }
    pthread_mutex_unlock(&start->set->pasta_mutex); 
    return(NULL);
}
