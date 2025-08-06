#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
//#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1

typedef enum e_activity_philosophers
{
  TAKEN_A_FORK,
  EATING,
  SLEEPING,
  THINKING,
  DIED,
  WAIT
} t_activity_philosophers;


typedef struct s_forks
{
    size_t          id;
    bool            available;
    pthread_mutex_t mutex;
    struct s_forks  *next;
}   t_forks;

typedef struct s_philo
{
    size_t id;
    long meals_eaten;
    long   t_alive;
    struct s_forks *left;
    struct s_forks *right;
    struct s_philo *next;
    struct s_settings *set;
    pthread_t thread_id;
} t_philo;

typedef struct s_settings
{
    long nbr_philo;
    long t_die;
    long t_eat;
    long t_sleep;
    time_t subunit;
    time_t subusec;
    time_t time_passed;
    long max_meal; //could place -1 to set at NONE instead of bool
    bool limit_meal;
    pthread_mutex_t death_mutex;
    pthread_mutex_t print_mutex;
    bool death;
} t_settings;

//debug.c
void print_id_related_forks(t_philo *philo, t_forks *forks);
void print_content_set(t_settings *set);

//utils.c
void create_philo(t_settings *settings, t_philo *philo, t_forks *forks);
int free_structs(t_settings *set,  t_philo *philo, t_forks *forks, int return_value);




#endif