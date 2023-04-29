#ifndef _proj2_h_
#define _proj2_h_

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
    sem_t urednik_sem[3];
    sem_t urednik_sem_done[3];
    unsigned int operation_num;
    unsigned int service_num[3];
    unsigned int post_open;
};

//opens all semaphores
void all_sem_open(struct Post *post_office);

//destroys initialized semaphores
void sem_clean(struct Post *post_office, int init_success);

//customer function
void zakaznik(int id, struct Post *post_office);

//officer function
void urednik(int id, struct Post *post_office);

#endif