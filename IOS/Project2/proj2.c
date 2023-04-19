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

struct Post{
    sem_t mutex;
    sem_t zakaznik_sem;
    sem_t urednik_sem;
    unsigned int operation_num;
    unsigned int service_num[3];
};



FILE *file;
//sem_t *mutex, *zakaznik_sem, *urednik_sem;
//unsigned int *operation_num, *service_num;
unsigned int NZ, NU, TZ, TU, F;



/*int zakaznik_ve_fronte()
{
    return 1;
}*/
int post_open()
{
    return 1;
}





void zakaznik(int id, struct Post *post_office)
{
    fprintf(stdout, "%d: Z %d: started\n", post_office->operation_num++, id);
    unsigned int uTZ = TZ * 1000;
    srand(time(0));
    sleep(rand() % (uTZ + 1)); //random number from 0 to uTZ

    if (post_open())
    {
        srand(time(0) + id * 777);
        unsigned int service = rand() % 3;
        post_office->service_num[service]++;

        fprintf(stdout, "%d: Z %d: entering office for a service %d\n", post_office->operation_num++, id, service + 1);
        
        sem_wait(&post_office->urednik_sem);

        fprintf(stdout, "%d: Z %d: called by office worker\n", post_office->operation_num++, id);
        
        srand(time(0));
        unsigned int waiting = (rand() % 11) * 1000; //random number from 0 to 10
        sleep(waiting);
    }
    fprintf(stdout, "%d: Z %d: going home\n", post_office->operation_num++, id);
    exit(0);
}


void urednik(int id, struct Post *post_office)
{
    fprintf(stdout, "%d: U %d: started\n", post_office->operation_num++, id);
    while(1)
    {
        if (post_office->service_num[0] || post_office->service_num[1] || post_office->service_num[2])
        {
            //bude se obsluhovat
            sem_post(&post_office->urednik_sem);
            unsigned int service;
            if (post_office->service_num[0])
            {
                service = 0;
            }
            else if (post_office->service_num[1])
            {
                service = 1;
            }
            else
            {
                service = 2;
            }
            post_office->service_num[service]--;
            
            fprintf(stdout, "%d: U %d: serving a service of type %d\n", post_office->operation_num++, id, service + 1);
            srand(time(0));
            unsigned int waiting = (rand() % 11) * 1000; //random number from 0 to 10
            sleep(waiting);
            fprintf(stdout, "%d: U %d: service finished\n", post_office->operation_num++, id);
            continue;
        }
        else if (post_open())
        {
            //urednik si bere prestavku
            fprintf(stdout, "%d: U %d: taking break\n", post_office->operation_num++, id);
            srand(time(0));
            unsigned uTU = TU * 1000;
            sleep(rand() % (uTU + 1)); //random number from 0 to uTU
            fprintf(stdout, "%d: U %d: break finished\n", post_office->operation_num++, id);
            continue;
        }
        else //post_not_open()
        {
            //urednik jde domu
            break;
        }
    }
    fprintf(stdout, "%d: U %d: going home\n", post_office->operation_num++, id);
    exit(0);
}


int main(int argc, char *argv[])
{
    struct Post *post_office = mmap(NULL, sizeof(struct Post), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (post_office == MAP_FAILED)
    {
        fprintf(stderr, "Error, mmap failed!\n");
        return 1;
    }
    post_office->operation_num = 1;
    post_office->service_num[0] = 0;
    post_office->service_num[1] = 0;
    post_office->service_num[2] = 0;


    //TODO fopen a fclose file imo
    setbuf(stdout, NULL); //TODO file misto stdout
    /*operation_num = mmap(NULL, sizeof(unsigned int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (operation_num == MAP_FAILED)
    {
        fprintf(stderr, "Error, mmap failed!\n");
        return 1;
    }
    *operation_num = 1;

    
    service_num = mmap(NULL, sizeof(unsigned int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (service_num == MAP_FAILED)
    {
        fprintf(stderr, "Error, mmap failed!\n");
        return 1;
    }*/




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
    



    //sdileni semaphoru
    /*mutex = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mutex == MAP_FAILED)
    {
        fprintf(stderr, "Error, mmap failed!\n");
        return 1;
    }
    zakaznik_sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (zakaznik_sem == MAP_FAILED)
    {
        fprintf(stderr, "Error, mmap failed!\n");
        return 1;
    }
    urednik_sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (urednik_sem == MAP_FAILED)
    {
        fprintf(stderr, "Error, mmap failed!\n");
        return 1;
    }*/


    //semaphore init
    int is_err = 0;
    //is_err = sem_init(mutex, 1, 1);
    is_err = sem_init((&post_office->mutex), 1, 1);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }
    //is_err = sem_init(zakaznik_sem, 1, 0);
    is_err = sem_init(&post_office->zakaznik_sem, 1, 0);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }
    /*is_err = sem_init(&post_office->zakaznik_sem[0], 1, 0);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }
    is_err = sem_init(&post_office->zakaznik_sem[1], 1, 0);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }
    is_err = sem_init(&post_office->zakaznik_sem[2], 1, 0);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }*/
    //is_err = sem_init(urednik_sem, 1, NU);
    is_err = sem_init(&post_office->urednik_sem, 1, NU);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }




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
            zakaznik(i, post_office);
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
            urednik(i, post_office);
        }
    }

    unsigned int uF = F * 1000; //milisecs to microsecs
    srand(time(0));
    sleep((rand() % (uF - uF / 2 + 1)) + uF / 2); //random number from uF / 2 to uF

    fprintf(stdout, "%d: closing\n", post_office->operation_num++);




    while(wait(NULL) > 0);

    //cisteni cleanup
    sem_destroy(&post_office->mutex); //? -1 pri erroru
    sem_destroy(&post_office->zakaznik_sem);
    /*sem_destroy(&post_office->zakaznik_sem[0]);
    sem_destroy(&post_office->zakaznik_sem[1]);
    sem_destroy(&post_office->zakaznik_sem[2]);*/
    sem_destroy(&post_office->urednik_sem);

    /*munmap(mutex, sizeof(sem_t)); //? -1 pri erroru
    munmap(operation_num, sizeof(unsigned int));
    munmap(service_num, sizeof(unsigned int));*/
    munmap(post_office, sizeof(struct Post));

    exit(0);
    return 0;
}
