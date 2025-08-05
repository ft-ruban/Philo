
#include <sys/time.h> //gettingtimeofdayneedit
#include "../include/philo.h"
#include <unistd.h> //usleep

void* routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    struct timeval tv;
//moitie wait la moitie de manger pour creer un decalage
    while(!philo->set->death)
    {
        printf("[%ld] %ld is thinking\n",philo->set->time_passed / 1000, philo->id);
        while(1)
        {
            if(philo->left->available && philo->right->available)
            {
                pthread_mutex_lock(&philo->left->mutex);
                philo->left->available = false;
                pthread_mutex_lock(&philo->right->mutex);
                philo->right->available = false;
                gettimeofday(&tv, NULL);
                philo->set->time_passed = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
                printf("[%ld] %ld has taken a fork\n",philo->set->time_passed / 1000, philo->id);
                philo->set->time_passed = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
                printf("[%ld] %ld is eating\n",philo->set->time_passed / 1000, philo->id);
                usleep(philo->set->t_eat);
                philo->left->available = true;
                pthread_mutex_unlock(&philo->left->mutex);
                philo->right->available = true;
                pthread_mutex_unlock(&philo->right->mutex);
                //reset time eat
                gettimeofday(&tv, NULL);
                philo->set->time_passed = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
                printf("[%ld] %ld is sleeping\n",philo->set->time_passed / 1000, philo->id);
                usleep(philo->set->t_sleep);
                break;
            }
            //TODO else if didnt ate in time
            //kill
            // gettimeofday(&tv, NULL);
            // philo->set->time_passed = (tv.tv_sec -  philo->set->subunit) * 1000000 + (tv.tv_usec - philo->set->subusec);
            // printf("[%ld] %ld died\n",philo->set->time_passed / 1000, philo->id);
        }
    }
    // pthread_mutex_lock(&philo->set->print_mutex);
    // pthread_mutex_unlock(&philo->set->print_mutex);
    return(0);
}

static void create_thread(t_philo *philo)
{
    while(philo)
    {
        if(pthread_create(&philo->thread_id, NULL, &routine, philo) != 0)
            return;
        philo = philo->next;
    }
}
static void join_thread(t_philo *philo)
{
    while(philo)
    {
        pthread_join(philo->thread_id, NULL); //protect?
        philo = philo->next;
    }
    
}

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