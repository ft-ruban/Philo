#ifndef PARSING_H
# define PARSING_H

#include "philo.h"
#include "stdbool.h"
#include <sys/time.h> //gettingtimeofdayneedit 

# define RETURN_ERROR 1
# define RETURN_SUCCESS 0
# define INVALID_INPUT -42

typedef enum e_error_status 
{
  INVALID_ARGC = 2,
  INVALID_PHILO,
  INVALID_DIE,
  INVALID_EAT,
  INVALID_SLEEP,
  INVALID_MAXMEAL,
} t_error_status;

//parsing.c
int     parsing(int argc, char *argv[], t_settings *set);

//parsing_utils.c
long	ft_atol(const char *nptr);

#endif