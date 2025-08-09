#ifndef PARSING_H
# define PARSING_H

#include "philo.h"
#include "stdbool.h"
#include <sys/time.h> //gettingtimeofdayneedit 

# define RETURN_ERROR 1
# define RETURN_SUCCESS 0
# define INVALID_INPUT -42
# define NO_MEAL -1

typedef enum e_error_status 
{
  INVALID_ARGC = 2,
  INVALID_PHILO,
  INVALID_DIE,
  INVALID_EAT,
  INVALID_SLEEP,
  INVALID_MAXMEAL,
  INVALID_SLEEP_PLUS_EAT,
} t_error_status;

//parsing_err_msg
int	error_msg(t_settings *set, int return_value);

//parsing.c
int     parsing(int argc, char *argv[], t_settings *set);

//parsing_utils.c
long	ft_atol(const char *nptr);
size_t	ft_strlen(const char *s);

#endif