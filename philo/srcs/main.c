#include "../include/philo.h"
#include <sys/time.h> //gettingtimeofdayneedit
#include <time.h> //time_t type need it
//parsing need between 1 and n philo also check if optional arg

#include <unistd.h> //USLEEP


int main(int argc, char *argv[])
{
    time_t test;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    test = tv.tv_sec;
    printf("Seconds since Epoch: %ld\n", tv.tv_sec - tv.tv_sec);
    printf("Microseconds part: %ld\n", tv.tv_usec);
    usleep(6000000);
    gettimeofday(&tv, NULL);
    printf("time : %ld\n", tv.tv_sec - test);
    if (parsing(argc, argv))
        printf("PARSING ERROR\n");
    else
        printf("PARSING SUCCESS\n");
    return (0);
}