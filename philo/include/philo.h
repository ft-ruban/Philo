#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
//#include <unistd.h>
#include <stdbool.h>
# define RETURN_ERROR 1
# define RETURN_SUCCESS 0

typedef struct s_forks
{
    bool        available;
    size_t      emplacement;
    struct s_forks  *next;
}   t_forks;

// typedef struct s_utils
// {

// }
#endif