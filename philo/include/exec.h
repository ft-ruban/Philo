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
//philo_utils.c
void join_thread(t_philo *philo);
void create_thread(t_philo *philo, t_settings *set);
void print_msg_routine(t_philo *philo, size_t cases);

//philo.c
void* routine_odd(void *arg);
void* routine_even(void *arg);
void philosopher(t_settings *set, t_philo *philo, t_forks *forks);

#endif