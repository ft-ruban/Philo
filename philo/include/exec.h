#ifndef EXEC_H
# define EXEC_H

#include "philo.h"

typedef enum e_msg_to_print
{
  IS_THINKING,
  IS_TAKING_FORK,
  IS_EATING,
  IS_SLEEPING,
} t_msg_to_print;



//philo_monitor.c
void* philo_monitor(void *arg);

//philo_routine.c
void* routine_odd(void *arg);
void* routine_even(void *arg);

//philo_utils.c
void print_msg_routine(t_philo *philo, size_t cases);
long fill_now();
void routine_take_fork(t_philo *philo, bool right);
//void routine_sleep(t_philo *philo, bool *first_iteration);

//philo.c
void* routine_odd(void *arg);
void* routine_even(void *arg);
void philosopher(t_settings *set, t_philo *philo, t_forks *forks);

#endif