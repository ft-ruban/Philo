#include "../include/philo.h"

//parsing need between 1 and n philo also check if optional arg

int parsing(int argc, char *argv[])
{
    printf("%s\n", argv[1]); //TODL
    if (argc > 6 || argc < 5)
        return(RETURN_ERROR);
    
    //TODO tokenisation for the received argv and save them into a struct
    //also check if the received args are numeric values I guess
    return(RETURN_SUCCESS);
}

int main(int argc, char *argv[])
{
    if (parsing(argc, argv))
        printf("PARSING ERROR\n");
    else
        printf("PARSING SUCCESS\n");
    return (0);
}