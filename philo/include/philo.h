#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
//#include <unistd.h>
#include <stdbool.h>

typedef struct s_forks
{
    bool        available;
    size_t      emplacement;
    struct s_forks  *next;
}   t_forks;

typedef struct s_philo
{

    size_t id;
    size_t meals_eaten;
    struct s_philo *next;
} t_philo;

typedef struct s_settings
{
    long nbr_philo;
    long t_die;
    long t_eat;
    long t_sleep;
    long max_meal;
    bool limit_meal;
    bool death;
} t_settings;

#endif