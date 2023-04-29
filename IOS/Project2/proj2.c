/*
*
* login: xblaze38
* last change: 29.4.2023
*
*/

#include "proj2.h"


FILE *file;
unsigned int NZ, NU, TZ, TU, F;


void all_sem_open(struct Post *post_office)
{
    sem_post(&post_office->mutex);
    sem_post(&post_office->urednik_sem[0]);
    sem_post(&post_office->urednik_sem[1]);
    sem_post(&post_office->urednik_sem[2]);
    sem_post(&post_office->urednik_sem_done[0]);
    sem_post(&post_office->urednik_sem_done[1]);
    sem_post(&post_office->urednik_sem_done[2]);
}


void sem_clean(struct Post *post_office, int init_success)
{
    switch (init_success)
    {
        case 7:
            sem_destroy(&post_office->urednik_sem_done[2]);
            sem_destroy(&post_office->urednik_sem_done[1]);
            sem_destroy(&post_office->urednik_sem_done[0]);
            sem_destroy(&post_office->urednik_sem[2]);
            sem_destroy(&post_office->urednik_sem[1]);
            sem_destroy(&post_office->urednik_sem[0]);
            sem_destroy(&post_office->mutex);
            break;
        case 6:
            sem_destroy(&post_office->urednik_sem_done[1]);
            sem_destroy(&post_office->urednik_sem_done[0]);
            sem_destroy(&post_office->urednik_sem[2]);
            sem_destroy(&post_office->urednik_sem[1]);
            sem_destroy(&post_office->urednik_sem[0]);
            sem_destroy(&post_office->mutex);
            break;
        case 5:
            sem_destroy(&post_office->urednik_sem_done[0]);
            sem_destroy(&post_office->urednik_sem[2]);
            sem_destroy(&post_office->urednik_sem[1]);
            sem_destroy(&post_office->urednik_sem[0]);
            sem_destroy(&post_office->mutex);
            break;
        case 4:
            sem_destroy(&post_office->urednik_sem[2]);
            sem_destroy(&post_office->urednik_sem[1]);
            sem_destroy(&post_office->urednik_sem[0]);
            sem_destroy(&post_office->mutex);
            break;
        case 3:
            sem_destroy(&post_office->urednik_sem[1]);
            sem_destroy(&post_office->urednik_sem[0]);
            sem_destroy(&post_office->mutex);
            break;
        case 2:
            sem_destroy(&post_office->urednik_sem[0]);
            sem_destroy(&post_office->mutex);
            break;
        case 1:
            sem_destroy(&post_office->mutex);
            break;
    } 
}


void zakaznik(int id, struct Post *post_office)
{
    sem_wait(&post_office->mutex);
    fprintf(file, "%d: Z %d: started\n", post_office->operation_num++, id);
    sem_post(&post_office->mutex);

    unsigned int uTZ = TZ * 1000;
    srand(time(0) + id * 666);
    usleep(rand() % (uTZ + 1)); //random number from 0 to uTZ


    sem_wait(&post_office->mutex);
    if (post_office->post_open)
    {
        //post is open
        srand(time(0) + id * 777);
        unsigned int service = rand() % 3;

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

        sem_wait(&post_office->urednik_sem_done[service]);

        sem_wait(&post_office->mutex);
        fprintf(file, "%d: Z %d: going home\n", post_office->operation_num++, id);
        sem_post(&post_office->mutex);
    }
    else
    {
        //post is not open
        fprintf(file, "%d: Z %d: going home\n", post_office->operation_num++, id);
        sem_post(&post_office->mutex);
    }
    exit(0);
}


void urednik(int id, struct Post *post_office)
{
    sem_wait(&post_office->mutex);
    fprintf(file, "%d: U %d: started\n", post_office->operation_num++, id);
    sem_post(&post_office->mutex);
    while(1)
    {
        sem_wait(&post_office->mutex);
        if (post_office->service_num[0] || post_office->service_num[1] || post_office->service_num[2])
        {
            //ceka zakaznik -> bude se obsluhovat
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

            sem_post(&post_office->urednik_sem_done[service]);

            sem_wait(&post_office->mutex);
            fprintf(file, "%d: U %d: service finished\n", post_office->operation_num++, id);
            sem_post(&post_office->mutex);
            continue;
        }
        else if (post_office->post_open)
        {
            //urednik si bere prestavku
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
            sem_post(&post_office->mutex);
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
        fclose(file);
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
        munmap(post_office, sizeof(struct Post));
        fclose(file);
        return 1;
    }
    
    for (int i = 1; i < 6; i++)
    {
        int j = 0;
        for (j = 0; argv[i][j] != '\0'; j++)
        {
            if (!isdigit(argv[i][j]))
            {
                fprintf(stderr, "Incomaptible arguments!\n");
                munmap(post_office, sizeof(struct Post));
                fclose(file);
                return 1;
            }
        }

        int tmp = atoi(argv[i]);
        if (tmp < 0)
        {
            fprintf(stderr, "Incomaptible arguments!\n");
            munmap(post_office, sizeof(struct Post));
            fclose(file);
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
                    munmap(post_office, sizeof(struct Post));
                    fclose(file);
                    return 1;
                }
                break;
            case 3:
                TZ = tmp;
                if (TZ > 10000)
                {
                    fprintf(stderr, "Incomaptible arguments!\n");
                    munmap(post_office, sizeof(struct Post));
                    fclose(file);
                    return 1;
                }
                break;
            case 4:
                TU = tmp;
                if (TU > 100)
                {
                    fprintf(stderr, "Incomaptible arguments!\n");
                    munmap(post_office, sizeof(struct Post));
                    fclose(file);
                    return 1;
                }
                break;
            case 5:
                F = tmp;
                if (F > 10000)
                {
                    fprintf(stderr, "Incomaptible arguments!\n");
                    munmap(post_office, sizeof(struct Post));
                    fclose(file);
                    return 1;
                }
                break;
        }       
    }
    

    //semaphore init
    int is_err = 0;
    int init_success = 0;
    is_err = sem_init(&post_office->mutex, 1, 1);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        sem_clean(post_office, init_success);
        munmap(post_office, sizeof(struct Post));
        fclose(file);
        return 1;
    }
    init_success++;

    is_err = sem_init(&post_office->urednik_sem[0], 1, NU);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        sem_clean(post_office, init_success);
        munmap(post_office, sizeof(struct Post));
        fclose(file);
        return 1;
    }
    init_success++;
    is_err = sem_init(&post_office->urednik_sem[1], 1, NU);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        sem_clean(post_office, init_success);
        munmap(post_office, sizeof(struct Post));
        fclose(file);
        return 1;
    }
    init_success++;
    is_err = sem_init(&post_office->urednik_sem[2], 1, NU);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        sem_clean(post_office, init_success);
        munmap(post_office, sizeof(struct Post));
        fclose(file);
        return 1;
    }
    init_success++;

    is_err = sem_init(&post_office->urednik_sem_done[0], 1, 0);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        sem_clean(post_office, init_success);
        munmap(post_office, sizeof(struct Post));
        fclose(file);
        return 1;
    }
    init_success++;
    is_err = sem_init(&post_office->urednik_sem_done[1], 1, 0);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        sem_clean(post_office, init_success);
        munmap(post_office, sizeof(struct Post));
        fclose(file);
        return 1;
    }
    init_success++;
    is_err = sem_init(&post_office->urednik_sem_done[2], 1, 0);
    if (is_err == -1)
    {
        fprintf(stderr, "Error, sem_init failed!\n");
        sem_clean(post_office, init_success);
        munmap(post_office, sizeof(struct Post));
        fclose(file);
        return 1;
    }
    init_success++;


    //main process
    for (unsigned int i = 1; i <= NZ; i++)
    {
        pid_t id = fork();
        if (id == -1)
        {
            fprintf(stderr, "Error with making child process!\n");
            for (unsigned int j = 0; j < i; j++) //wait until all processes finish
            {
                all_sem_open(post_office);
            }
            while(wait(NULL) > 0);
            sem_clean(post_office, init_success);
            munmap(post_office, sizeof(struct Post));
            fclose(file);
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
            for (unsigned int j = 0; j < (NZ + i); j++) //wait until all processes finish
            {
                all_sem_open(post_office);
            }
            while(wait(NULL) > 0);
            sem_clean(post_office, init_success);
            munmap(post_office, sizeof(struct Post));
            fclose(file);
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
    sem_clean(post_office, init_success);

    munmap(post_office, sizeof(struct Post));

    fclose(file);
    return 0;
}
