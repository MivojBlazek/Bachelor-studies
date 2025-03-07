#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define NUM_CHAIRS 5
#define NUM_CUSTOMERS 20
#define NUM_BARBERS 3

struct barbershop {
    sem_t customers;
    sem_t barbers;
    sem_t mutex;
    int num_waiting;
};

int cut_choice;

int main() {
    
    cut_choice = *((int *) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));

    // Allocate shared memory for the barbershop data
    struct barbershop *shop = mmap(NULL, sizeof(struct barbershop), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    shop->num_waiting = 0;

    // Initialize semaphores
    sem_init(&shop->customers, 1, 0);
    sem_init(&shop->barbers, 1, NUM_BARBERS);
    sem_init(&shop->mutex, 1, 1);

    // Fork child processes to act as barbers
    for (int i = 0; i < NUM_BARBERS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            while (1) {
                sem_wait(&shop->customers);
                sem_wait(&shop->mutex);
                // cut_choice = *((int *) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0));
                shop->num_waiting--;
                sem_post(&shop->barbers);
                sem_post(&shop->mutex);
                printf("Barber is cutting hair according to cut %d.\n", cut_choice);
                srand(time(NULL));
                sleep((rand() % 5 + 1) / 10); // Simulate haircut time
            }
        } else if (pid < 0) {
            fprintf(stderr, "Failed to fork barber process %d.\n", i);
            exit(1);
        }
    }

    // Parent process acts as customer generator
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pid_t customer_pid = fork();
        if (customer_pid == 0) {
            srand(time(NULL) + i * 777);
            cut_choice = (rand() % 3) + 1; // Randomly choose a haircut option between 1 and 3
            printf("Customer %d arrived at the barbershop and chose haircut %d.\n", i, cut_choice);
            sem_wait(&shop->mutex);
            if (shop->num_waiting < NUM_CHAIRS) {
                shop->num_waiting++;
                sem_post(&shop->customers);
                sem_post(&shop->mutex);
                sem_wait(&shop->barbers);
                //memcpy(mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0), &cut_choice, sizeof(int));
                printf("Customer %d is getting a haircut.\n", i);
                srand(time(NULL));
                sleep((rand() % 5 + 1) / 10); // Simulate haircut time
                printf("Customer %d is done with the haircut.\n", i);
            } else {
                sem_post(&shop->mutex);
                printf("Customer %d could not get a haircut and leaves.\n", i);
            }
            exit(0);
        } else if (customer_pid < 0) {
            fprintf(stderr, "Failed to fork customer process %d.\n", i);
            exit(1);
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        wait(NULL);
    }

    return 0;
}