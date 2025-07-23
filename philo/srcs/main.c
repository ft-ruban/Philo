#include "../include/philo.h"
#include <sys/time.h> //gettingtimeofdayneedit
#include <time.h> //time_t type need it
//parsing need between 1 and n philo also check if optional arg



int main(int argc, char *argv[])
{
    //time_t seconds;
    // struct timeval tv;

    // gettimeofday(&tv, NULL);
    // printf("Seconds since Epoch: %ld\n", tv.tv_sec);
    // printf("Microseconds part: %ld\n", tv.tv_usec);
    if (parsing(argc, argv))
        printf("PARSING ERROR\n");
    else
        printf("PARSING SUCCESS\n");
    return (0);
}