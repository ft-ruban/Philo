#ifndef EXEC_H
# define EXEC_H

#include "philo.h"

//philo_utils.c
void join_thread(t_philo *philo);
void create_thread(t_philo *philo);

//philo.c
void* routine_odd(void *arg);
void* routine_even(void *arg);
void philosopher(t_settings *set, t_philo *philo, t_forks *forks);

#endif