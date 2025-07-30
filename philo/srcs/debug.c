#include <limits.h>
#include "../include/philo.h"

void print_id_related_forks(t_philo *philo, t_forks *forks)
{
    while(philo)
    {
        printf("philo_id : %zu\n",philo->id);
        printf("fork_left = : %zu\n",philo->left->id);
        printf("fork_right = : %zu\n",philo->right->id);
        philo = philo->next;
    }
    while(forks)
    {
        printf("fork_id : %zu\n",forks->id);
        forks = forks->next;
    }
}

void print_content_set(t_settings *set)
{
    printf("longmax = %ld\n",LONG_MAX);
	printf("nbr_philo = %ld\n", set->nbr_philo);
	printf("t_die = %ld\n", set->t_die);
	printf("t_eat = %ld\n", set->t_eat);
	printf("t_sleep = %ld\n", set->t_sleep);
}