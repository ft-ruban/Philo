
#include "../include/parsing.h"

// int convert(char *argv[])
// {
    
// }

int parsing(int argc, char *argv[])
{
    printf("%s\n", argv[1]); //TODL
    if (argc > 6 || argc < 5)
        return(RETURN_ERROR);
    
    //TODO converts for the received argv and save them into a struct
    //also check if the received args are numeric values I guess
    return(RETURN_SUCCESS);
}