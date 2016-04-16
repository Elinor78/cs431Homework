#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define N 100		// number of slots in the buffer
#define TRUE 1
#define FALSE 0

sem_t empty;
int count = 0;


int produce_item() {
	//printf("Produced item: %d\n", count);
	return count;
}

int remove_item() {
	printf("Removed item: %d\n", count);
	return count;
}

void consume_item(int item) {
	printf("Consumed item: %d\n", item);
	return;
}

void insert_item(int item) {
	printf("Inserted item: %d\n", item);
	return;
}	


void* producer (void* ptr) {
	int item;

	while (TRUE) {
		item = produce_item();

		sem_wait(&empty);		// empty initially = N; if value of semaphore is zero, calling process blocks

		if (count < N) {				// enter critical region
			count = count + 1;
			insert_item(item);	// put new item in buffer
		}							// leave critical region

		sem_post(&empty);		// increments value of mutex, wakes up blocked process waiting on semaphore
	}
	return NULL;
}

void* consumer (void *ptr) {
	int item;
	
	while (TRUE) {	
		sem_wait(&empty);			// empty initially = N; if value of semaphore is zero, calling process blocks

		if (count > 0) {					// enter critical region
			count = count - 1;
			item = remove_item();	// remove item from buffer
			consume_item(item);		// print item
		}								// leave critical region

		sem_post(&empty);
	}
	return NULL;
}


int main() {
	
	sem_init(&empty, 0, N);	// empty = N empty slots in buffer

  	pthread_t cons, prod;
	pthread_create (&cons, NULL, &consumer, NULL);
	pthread_create (&prod, NULL, &producer, NULL);
    	
	while(1)
	fputs("", stderr);
	return 0;
}








