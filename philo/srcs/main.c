#include "../include/philo.h"
#include <sys/time.h> //gettingtimeofdayneedit
#include <time.h> //time_t type need it
//parsing need between 1 and n philo also check if optional arg
#include <stdlib.h> //malloc
#include "parsing.h"

int main(int argc, char *argv[])
{
    t_settings *set;

    set = malloc(sizeof (t_settings));
    if(!set)
        return (EXIT_FAILURE);
    if (parsing(argc, argv, set))
    {
        printf("PARSING ERROR\n");
        return(RETURN_ERROR);
    }
    else
        printf("PARSING SUCCESS\n");
    free(set);
    return (0);
}