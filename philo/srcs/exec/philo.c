
#include <sys/time.h> //gettingtimeofdayneedit
#include "exec.h"
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
void* routine_odd(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    //struct timeval tv;
    //TODO mettre le t_eat /2 directement dans la struct pour eviter calcul redondant
    usleep(1000);//moitie wait la moitie de manger pour creer un decalage
    pthread_mutex_lock(&philo->set->death_mutex);
    while(!philo->set->death && philo->meals_eaten != philo->set->max_meal)
    {
        pthread_mutex_unlock(&philo->set->death_mutex);
        print_msg_routine(philo, IS_THINKING);
        usleep(250);
        routine_take_fork(philo, true);
        routine_take_fork(philo, false);
        print_msg_routine(philo, IS_EATING);
        //usleep_routine(philo->set->t_eat, philo);
        if(philo->set->t_eat > philo->set->t_die)
            usleep(philo->set->t_die);
        else
            usleep(philo->set->t_eat);
        philo->left->available = true;
        pthread_mutex_unlock(&philo->left->mutex);
        philo->right->available = true;
        pthread_mutex_unlock(&philo->right->mutex);
        print_msg_routine(philo, IS_SLEEPING);
        if(philo->set->t_eat + philo->set->t_sleep > philo->set->t_die)
            usleep(philo->set->t_die);
        else
        usleep(philo->set->t_eat);
        //usleep_routine(philo->set->t_sleep, philo);
        //usleep(philo->set->t_sleep);   
        pthread_mutex_lock(&philo->set->death_mutex);
    }
    pthread_mutex_unlock(&philo->set->death_mutex);
    pthread_mutex_lock(&philo->set->pasta_mutex); 
    if(philo->meals_eaten == philo->set->max_meal)
        philo->set->philo_full_pasta = philo->set->philo_full_pasta + 1;
    pthread_mutex_unlock(&philo->set->pasta_mutex); 
    return(0);
}

void* routine_even(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    pthread_mutex_lock(&philo->set->death_mutex);
    while(!philo->set->death && philo->meals_eaten != philo->set->max_meal)
    {
        pthread_mutex_unlock(&philo->set->death_mutex);
        print_msg_routine(philo, IS_THINKING);
        usleep(250);
        routine_take_fork(philo, false);
        if(philo->right->id == philo->left->id)
        {
            pthread_mutex_lock(&philo->set->death_mutex);
            pthread_mutex_unlock(&philo->left->mutex); 
            break;
            // while(!philo->set->death)
            //     print_msg_routine(philo, WAIT);
            
        }
        routine_take_fork(philo, true);
        print_msg_routine(philo, IS_EATING);
        if(philo->set->t_eat > philo->set->t_die)
            usleep(philo->set->t_die);
        else
        usleep(philo->set->t_eat);
        //usleep_routine(philo->set->t_eat, philo);
        //usleep(philo->set->t_eat);
        philo->left->available = true;
        pthread_mutex_unlock(&philo->left->mutex);
        philo->right->available = true;
        pthread_mutex_unlock(&philo->right->mutex);
        print_msg_routine(philo, IS_SLEEPING);
        if(philo->set->t_eat + philo->set->t_sleep > philo->set->t_die)
            usleep(philo->set->t_die);
        else
        usleep(philo->set->t_eat);
        //usleep_routine(philo->set->t_sleep, philo);
        //usleep(philo->set->t_sleep);  
        pthread_mutex_lock(&philo->set->death_mutex);  
    }
    pthread_mutex_unlock(&philo->set->death_mutex);
    pthread_mutex_lock(&philo->set->pasta_mutex); 
    if(philo->meals_eaten == philo->set->max_meal)
        philo->set->philo_full_pasta = philo->set->philo_full_pasta + 1;
    pthread_mutex_unlock(&philo->set->pasta_mutex);
    return(0);
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