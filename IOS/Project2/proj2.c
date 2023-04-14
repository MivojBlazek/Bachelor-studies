#include <stdio.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/mman.h>
#include <time.h>

FILE *file;
sem_t *mutex;
unsigned int *operation_num, *service_num;
unsigned int NZ, NU, TZ, TU, F;


void zakaznik(int id)
{
    fprintf(stdout, "%d: Z %d: started\n", *operation_num++, id);
    unsigned int uTZ = TZ * 1000;
    srand(time(0));
    sleep(rand() % (uTZ + 1)); //random number from 0 to uTZ

    if (post_open())
    {
        srand(time(0));

        service_num = mmap(NULL, sizeof(unsigned int), PROT_READ | PROT_WRITE, MAP_SHARED, -1, 0);
        if (service_num == MAP_FAILED)
        {
            fprintf(stderr, "Error, mmap failed!\n");
            return 1;
        }
        *service_num = (rand() % 3) + 1;

        fprintf(stdout, "%d: Z %d: entering office for a service %d\n", *operation_num++, id, *service_num);
        //TODO zaradit do fronty service_num a pockat na urednika

        fprintf(stdout, "%d: Z %d: called by office worker\n", *operation_num++, id);
        
        srand(time(0));
        unsigned int waiting = (rand() % 11) * 1000; //random number from 0 to 10
        sleep(waiting);
    }
    fprintf(stdout, "%d: Z %d: going home\n", *operation_num++, id);
}


void urednik(int id)
{
    fprintf(stdout, "%d: U %d: started\n", *operation_num++, id);
    while(1)
    {
        if (zakaznik_ve_fronte())
        {
            //bude se obsluhovat
            fprintf(stdout, "%d: U %d: serving a service of type %d\n", *operation_num++, id, *service_num);
            srand(time(0));
            unsigned int waiting = (rand() % 11) * 1000; //random number from 0 to 10
            sleep(waiting);
            fprintf(stdout, "%d: U %d: service finished\n", *operation_num++, id);
            continue;
        }
        if (post_open() && !zakaznik_ve_fronte())
        {
            //urednik si bere prestavku
            fprintf(stdout, "%d: U %d: taking break\n", *operation_num++, id);
            srand(time(0));
            unsigned uTU = TU * 1000;
            sleep(rand() % (uTU + 1)); //random number from 0 to uTU
            fprintf(stdout, "%d: U %d: break finished\n", *operation_num++, id);
            continue;
        }
        if (!post_open() && !zakaznik_ve_fronte())
        {
            //urednik jde domu
            break;
        }
    }
    fprintf(stdout, "%d: U %d: going home\n", *operation_num++, id);
}


int main(int argc, char *argv[])
{
    setbuf(file, NULL);
    operation_num = mmap(NULL, sizeof(unsigned int), PROT_READ | PROT_WRITE, MAP_SHARED, -1, 0);
    if (operation_num == MAP_FAILED)
    {
        fprintf(stderr, "Error, mmap failed!\n");
        return 1;
    }
    *operation_num = 1;

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
    



//TODO hrani si s mmap a sem
    mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, -1, 0);
    if (mutex == MAP_FAILED)
    {
        fprintf(stderr, "Error, mmap failed!\n");
        return 1;
    }
    munmap(mutex, sizeof(sem_t)); //? -1 pri erroru

    int is_err = sem_init(mutex, 1, 0); //semaphore init
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }

    sem_destroy(mutex); //semaphore destroy //? -1 pri erroru
//TODO hrani si s mmap a sem





    //main process
    for (unsigned int i = 1; i <= NZ; i++)
    {
        pid_t id = fork();
        if (id == -1)
        {
            fprintf(stderr, "Error with making child process!\n");
            return 1;
        }
        else if (id == 0)
        {
            zakaznik(i);
            exit(0);
        }
    }
    for (unsigned int i = 1; i <= NU; i++)
    {
        pid_t id = fork();
        if (id == -1)
        {
            fprintf(stderr, "Error with making child process!\n");
            return 1;
        }
        else if (id == 0)
        {
            urednik(i);
            exit(0);
        }
    }

    unsigned int uF = F * 1000; //milisecs to microsecs
    srand(time(0));
    sleep((rand() % (uF - uF / 2 + 1)) + uF / 2); //random number from uF / 2 to uF

    fprintf(stdout, "%d: closing\n", *operation_num++);




    while(wait(NULL) > 0);

    //cisteni cleanup

    exit(0);
    return 0;
}
