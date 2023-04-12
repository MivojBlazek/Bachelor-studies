#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>


int main(int argc, char **argv)
{
    int NZ, NU, TZ, TU, F;


    if (argc != 6)
    {
        fprintf(stderr, "Incomaptible arguments!\n");
        return 1;
    }
    
    for (int i = 1; i < 6; i++)
    {
        int tmp = atoi(argv[i]);
        if (tmp < 0)
        {
            fprintf(stderr, "Incomaptible arguments!\n");
            return 1;
        }
        switch (i)
        {
            case 1:
                NZ = tmp;
                break;
            case 2:
                NU = tmp;
                break;
            case 3:
                TZ = tmp;
                if (TZ > 10000)
                {
                    fprintf(stderr, "Incomaptible arguments!\n");
                    return 1;
                }
                break;
            case 4:
                TU = tmp;
                if (TU > 100)
                {
                    fprintf(stderr, "Incomaptible arguments!\n");
                    return 1;
                }
                break;
            case 5:
                F = tmp;
                if (F > 10000)
                {
                    fprintf(stderr, "Incomaptible arguments!\n");
                    return 1;
                }
                break;
        }       
    }
    

    return 0;
}
