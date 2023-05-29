#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5 // Number of available chairs

sem_t mutex;      	// Mutex semaphore for mutual exclusion
sem_t customers;  	// Semaphore to track the number of customers
sem_t barber;     	// Semaphore to signal the barber when a customer arrives
int waiting_count = 0;  // Number of customers waiting

void leave() {
    // Customer leaves the shop permanently
    printf("Customer leaving the shop.\n");
    pthread_exit(NULL);
}

void getHairCut() {
    // Customer gets a haircut
    printf("Customer getting a haircut.\n");
}

void cutHair() {
    // Barber cuts hair
    printf("Barber cutting hair.\n");
}

void *customer_thread(void *arg) {

    // Checking if there is a free chair
    sem_wait(&mutex);

    if (waiting_count < N) {	   // Seat is available

        waiting_count++;
        sem_post(&customers);	// Tells barber that there is a new customer
        sem_post(&mutex);

        // Wait for the barber
        sem_wait(&barber);   // Customer waits for the barber to be available
        getHairCut();

    } else {

        // No free chair, customer leaves
        sem_post(&mutex);
        leave();
    }

    pthread_exit(NULL);
}

void *barber_thread(void *arg) {
    while (1) {

        sem_wait(&customers); // Barber sleeps, waiting for a customer to arrive

        // Barber wakes up
        sem_wait(&mutex);
        waiting_count--;     // Signals the availability of the barber to the customer
        sem_post(&barber);   // Customer is notified that barber is available
        sem_post(&mutex);

        // Barber cuts hair
        cutHair();
    }

    pthread_exit(NULL);
}

int main() {
    int i;
    pthread_t barber_th;
    pthread_t customer_th[N];
    int customer_id[N];

    // Initializing semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&customers, 0, 0);
    sem_init(&barber, 0, 0);

    // Creating barber thread
    pthread_create(&barber_th, NULL, barber_thread, NULL);

    // Creating customer threads
    for (i = 0; i < N; i++) {
        customer_id[i] = i + 1;
        pthread_create(&customer_th[i], NULL, customer_thread, (void*)&customer_id[i]);
    }

    // Join barber thread
    pthread_join(barber_th, NULL);

    // Join customer threads
    for (i = 0; i < N; i++) {
        pthread_join(customer_th[i], NULL);
    }

    // Destroying semaphores
    sem_destroy(&mutex);
    sem_destroy(&customers);
    sem_destroy(&barber);

    return 0;
}

