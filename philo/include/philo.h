#ifndef PHILO_H
# define PHILO_H

#include "parsing.h"
#include <stdio.h>
//#include <unistd.h>
#include <stdbool.h>


typedef struct s_forks
{
    bool        available;
    size_t      emplacement;
    struct s_forks  *next;
}   t_forks;

typedef struct s_utils
{
    size_t nbr_philo;
    size_t time_to_die;
} t_utils;

#endif