#include "exec.h"

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