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
#include <ctype.h>

struct Post{
    sem_t mutex;
    sem_t urednik_sem_done[3]; //? not sure
    sem_t urednik_sem[3];
    unsigned int operation_num;
    unsigned int service_num[3];
    unsigned int post_open;
};



FILE *file;
unsigned int NZ, NU, TZ, TU, F;



void zakaznik(int id, struct Post *post_office)
{
    sem_wait(&post_office->mutex);
    fprintf(file, "%d: Z %d: started\n", post_office->operation_num++, id);
    sem_post(&post_office->mutex);

    unsigned int uTZ = TZ * 1000;
    srand(time(0) + id * 666);
    usleep(rand() % (uTZ + 1)); //random number from 0 to uTZ


    if (post_office->post_open)
    {
        //post is open
        srand(time(0) + id * 777);
        unsigned int service = rand() % 3;

        sem_wait(&post_office->mutex);
        post_office->service_num[service]++;
        fprintf(file, "%d: Z %d: entering office for a service %d\n", post_office->operation_num++, id, service + 1);
        sem_post(&post_office->mutex);
        
        sem_wait(&post_office->urednik_sem[service]);

        sem_wait(&post_office->mutex);
        fprintf(file, "%d: Z %d: called by office worker\n", post_office->operation_num++, id);
        sem_post(&post_office->mutex);

        srand(time(0));
        unsigned int waiting = rand() % 11; //random number from 0 to 10
        usleep(waiting);

        sem_wait(&post_office->urednik_sem_done[service]); //? not sure
    }
    sem_wait(&post_office->mutex);
    fprintf(file, "%d: Z %d: going home\n", post_office->operation_num++, id);
    sem_post(&post_office->mutex);
    exit(0);
}


void urednik(int id, struct Post *post_office)
{
    sem_wait(&post_office->mutex);
    fprintf(file, "%d: U %d: started\n", post_office->operation_num++, id);
    sem_post(&post_office->mutex);
    while(1)
    {
        if (post_office->service_num[0] || post_office->service_num[1] || post_office->service_num[2])
        {
            //ceka zakaznik -> bude se obsluhovat
            sem_wait(&post_office->mutex);
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
            sem_post(&post_office->urednik_sem[service]);
            post_office->service_num[service]--;
            fprintf(file, "%d: U %d: serving a service of type %d\n", post_office->operation_num++, id, service + 1);
            sem_post(&post_office->mutex);

            srand(time(0));
            unsigned int waiting = rand() % 11; //random number from 0 to 10
            usleep(waiting);

            sem_post(&post_office->urednik_sem_done[service]); //? not sure

            sem_wait(&post_office->mutex);
            fprintf(file, "%d: U %d: service finished\n", post_office->operation_num++, id);
            sem_post(&post_office->mutex);
            continue;
        }
        else if (post_office->post_open)
        {
            //urednik si bere prestavku
            sem_wait(&post_office->mutex);
            fprintf(file, "%d: U %d: taking break\n", post_office->operation_num++, id);
            sem_post(&post_office->mutex);

            srand(time(0));
            unsigned int uTU = TU * 1000;
            usleep(rand() % (uTU + 1)); //random number from 0 to uTU

            sem_wait(&post_office->mutex);
            fprintf(file, "%d: U %d: break finished\n", post_office->operation_num++, id);
            sem_post(&post_office->mutex);
            continue;
        }
        else //post not open
        {
            //urednik jde domu
            break;
        }
    }
    sem_wait(&post_office->mutex);
    fprintf(file, "%d: U %d: going home\n", post_office->operation_num++, id);
    sem_post(&post_office->mutex);
    exit(0);
}


int main(int argc, char *argv[])
{
    file = fopen("proj2.out", "w");
    if (file == NULL)
    {
        fprintf(stderr, "Error, fopen failed!\n");
        return 1;
    }

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
    post_office->post_open = 1;


    setbuf(file, NULL);



    //program arguments
    if (argc != 6)
    {
        fprintf(stderr, "Incomaptible arguments!\n");
        return 1;
    }
    
    for (int i = 1; i < 6; i++)
    {
        int j = 0;
        if (argv[i][0] == '-')
        {
            fprintf(stderr, "Incomaptible arguments!\n");
            return 1;
        }
        for (j = 0; argv[i][j] != '\0'; j++)
        {
            if (!isdigit(argv[i][j]))
            {
                fprintf(stderr, "Incomaptible arguments!\n");
                return 1;
            }
        }

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
                if (NU == 0)
                {
                    fprintf(stderr, "Incomaptible arguments!\n");
                    return 1;
                }
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
    

    //semaphore init
    int is_err = 0;
    is_err = sem_init((&post_office->mutex), 1, 1);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }
    /*is_err = sem_init(&post_office->zakaznik_sem, 1, 0);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }*/
    is_err = sem_init(&post_office->urednik_sem[0], 1, NU);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }
    is_err = sem_init(&post_office->urednik_sem[1], 1, NU);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }
    is_err = sem_init(&post_office->urednik_sem[2], 1, NU);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }

    is_err = sem_init(&post_office->urednik_sem_done[0], 1, NU); //? not sure
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }
    is_err = sem_init(&post_office->urednik_sem_done[1], 1, NU); //? not sure
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        return 1;
    }
    is_err = sem_init(&post_office->urednik_sem_done[2], 1, NU); //? not sure
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
    usleep((rand() % (uF - uF / 2 + 1)) + uF / 2); //random number from uF / 2 to uF

    sem_wait(&post_office->mutex);
    fprintf(file, "%d: closing\n", post_office->operation_num++);
    post_office->post_open = 0;
    sem_post(&post_office->mutex);


    while(wait(NULL) > 0);


    //cleaning
    sem_destroy(&post_office->mutex);
    //sem_destroy(&post_office->zakaznik_sem);
    sem_destroy(&post_office->urednik_sem[0]);
    sem_destroy(&post_office->urednik_sem[1]);
    sem_destroy(&post_office->urednik_sem[2]);
    
    sem_destroy(&post_office->urednik_sem_done[0]); //? not sure
    sem_destroy(&post_office->urednik_sem_done[1]); //? not sure
    sem_destroy(&post_office->urednik_sem_done[2]); //? not sure

    munmap(post_office, sizeof(struct Post));

    fclose(file);
    exit(0);
    return 0;
}
