#include <pthread.h>		// for pthread_t, pthread_create, pthread_join
#include <stdio.h>			// for print_f
#include <semaphore.h>

#define N 100				// number of slots in buffer
#define TRUE 1
#define FALSE 0
int buffer[N];
sem_t empty_sem;
sem_t full_sem;

void produce_item(int i) {
	buffer[i] = i;
	printf("Produced item %d, inserted in buffer slot %d\n", buffer[i], i);
}

int consume_item(int i) {	
	printf("Consumed item %d, removed from buffer slot %d\n", buffer[i], i);
	int item = buffer[i];
	buffer[i] = -1;

	return item;
}

void *producer(void *ptr) {
		int i;
		for (i = 0; i < N; i++) {
				sem_wait(&empty_sem);
				produce_item(i);
				sem_post(&full_sem);
		}
}

void *consumer(void *ptr) {
		int i;
		for (i = 0; i < N; ++i) {
				sem_wait(&full_sem);
				consume_item(i);
				sem_post(&empty_sem);
		}
}


int main(int argc, char const *arg[]) {
		sem_init(&empty_sem, 0, N);
		sem_init(&full_sem, 0, 0);

		pthread_t cons_thread, prod_thread;
		pthread_create(&prod_thread, NULL, producer, NULL);
		pthread_create(&cons_thread, NULL, consumer, NULL);
		pthread_join(prod_thread, 0);
		pthread_join(cons_thread, 0);

		sem_destroy(&empty_sem);
		sem_destroy(&full_sem);

		return 0;
}
