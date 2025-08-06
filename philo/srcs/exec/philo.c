
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
    usleep(250);//moitie wait la moitie de manger pour creer un decalage
    while(!philo->set->death && philo->meals_eaten != philo->set->max_meal)
    {
        print_msg_routine(philo, IS_THINKING);
        usleep(250);
        routine_take_fork(philo, true);
        routine_take_fork(philo, false);
        print_msg_routine(philo, IS_EATING);
        usleep(philo->set->t_eat);
        philo->left->available = true;
        pthread_mutex_unlock(&philo->left->mutex);
        philo->right->available = true;
        pthread_mutex_unlock(&philo->right->mutex);
        print_msg_routine(philo, IS_SLEEPING);
        usleep(philo->set->t_sleep);   
    }
    return(0);
}

void* routine_even(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    while(!philo->set->death && philo->meals_eaten != philo->set->max_meal)
    {
        print_msg_routine(philo, IS_THINKING);
        usleep(250);
        routine_take_fork(philo, false);
        if(philo->right->id == philo->left->id)
        {
            while(!philo->set->death)
                print_msg_routine(philo, WAIT);
            break;
        }
        routine_take_fork(philo, true);
        print_msg_routine(philo, IS_EATING);
        usleep(philo->set->t_eat);
        philo->left->available = true;
        pthread_mutex_unlock(&philo->left->mutex);
        philo->right->available = true;
        pthread_mutex_unlock(&philo->right->mutex);
        print_msg_routine(philo, IS_SLEEPING);
        usleep(philo->set->t_sleep);    
        }
    return(0);
}
    // pthread_mutex_lock(&philo->set->print_mutex);
    // pthread_mutex_unlock(&philo->set->print_mutex);

void philosopher(t_settings *set, t_philo *philo, t_forks *forks)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    set->subunit = tv.tv_sec;
    set->subusec = tv.tv_usec;
    //usleep(5006000);
    gettimeofday(&tv, NULL);
    set->time_passed = (tv.tv_sec -  set->subunit) * 1000000 + (tv.tv_usec - set->subusec);
    // printf("result = : %ld \n",set->time_passed / 1000);
    create_thread(philo);
    join_thread(philo);
    set = NULL;
    philo = NULL;
    forks = NULL;
   // printf("nice");
    // thread_maker(set, philo, forks);
    //while(!set->death)
}