/*
	USAGE: ./1aproc 4 2 3 3000

	SEMAPHORES implementation
	BufferPrinter prints the status of the buffers.
	Accept arguments from the shell using main(argc, argv), allow input: # producers, # consumers, # buffers, # items
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#define TRUE 1
#define FALSE 0
#define ITEMS_PER_BUFFER 1024
int previously_producing = FALSE;

sem_t empty_sem;
sem_t full_sem;
sem_t mutex_sem;

int **buffer;
int producer_buffer = 0;	// current buffer in use by producer
int consumer_buffer = 0;	// current buffer in use by consumer
int producer_index = 0;		// current index in producer's buffer
int consumer_index = 0;		// current index in consumer's buffer

long produced = 0;			// total number of items produced
long consumed = 0;			// total number of items consumed
long num_producers;			// number of initialized producers
long num_consumers;			// number of initialized consumers
long num_buffers;			// number of initialized shared buffers
long num_items;				// number of items to create
int count = 0;				// ith item


void count_buffer_items() {
	int temp_consumed = consumed % (num_buffers * ITEMS_PER_BUFFER);
	int temp_produced = produced % (num_buffers * ITEMS_PER_BUFFER);
	int in_current_buffer = 0;

	int j;
	for (j = 0; j < num_buffers; j++) {
		int in_buffer_produced = 0;
		int in_buffer_consumed = 0;

		// determine number of items produced in buffer
		if (temp_produced >= ITEMS_PER_BUFFER) {
			in_buffer_produced = ITEMS_PER_BUFFER;
			temp_produced = temp_produced - ITEMS_PER_BUFFER;
		}
		else {
			in_buffer_produced = temp_produced;
			temp_produced = 0;
		}

		// determine number of items consumed in buffer
		if (temp_consumed >= ITEMS_PER_BUFFER) {
			in_buffer_consumed = ITEMS_PER_BUFFER;
			temp_consumed = temp_consumed - ITEMS_PER_BUFFER;
		}
		else {
			in_buffer_consumed = temp_consumed;
			temp_consumed = 0;
		}

		// determine number of items left in buffer
		if ((consumed % (num_buffers * ITEMS_PER_BUFFER)) < produced % (num_buffers * ITEMS_PER_BUFFER)) {
			in_current_buffer = in_buffer_produced - in_buffer_consumed;
		}
		else {
			in_current_buffer = ITEMS_PER_BUFFER - abs(in_buffer_produced - in_buffer_consumed);
		}

		printf("Shared buffer %d has %d items\n", j, in_current_buffer);
	}
	
}


void *producer(void *ptr){
	int producer_num = (int) ptr;		// current producer number

	while (produced < num_items) {

		sem_wait(&empty_sem);
		sem_wait(&mutex_sem);

		// no more items to consume
		if (producer_buffer == consumer_buffer && producer_index == consumer_index && !previously_producing && produced < num_items) {
			previously_producing = 1;
		}
		else if (produced < num_items) {
			buffer[producer_buffer][producer_index] = count;
			
			printf("Producer %d put %d in buffer %d, slot %d\n", 
				producer_num, buffer[producer_buffer][producer_index], producer_buffer, (int)producer_index);
			produced++;
			producer_index++;
			count++;

			if (produced % 1000 == 0) {
				printf("%d items created\n", (int)produced);
				count_buffer_items();
			}
		}

		// go to next shared buffer
		if (producer_index == ITEMS_PER_BUFFER) {
			producer_buffer++;
			producer_buffer = producer_buffer % num_buffers;
			producer_index = 0;
		}

		if (produced == num_items) {
			printf("Producer Thread %d is finished\n", producer_num);
		}

		sem_post(&mutex_sem);
		sem_post(&full_sem);

	}	

}

void *consumer(void *ptr){
	int consumer_num = (int) ptr;		// current consumer number

	while (consumed <= produced && consumed < num_items) {

		sem_wait(&full_sem);
		sem_wait(&mutex_sem);

		if (consumed < produced) {
			printf("Consumer %d took %d from buffer %d, slot %d\n", 
				consumer_num, buffer[consumer_buffer][consumer_index], consumer_buffer, (int)consumer_index);
			buffer[consumer_buffer][consumer_index] = 0;
			consumed++;
			consumer_index++;

			if (consumer_index == ITEMS_PER_BUFFER) {
				consumer_buffer++;
				consumer_buffer = consumer_buffer % num_buffers;
				consumer_index = 0;
			}
		}

		// no more items to consume
		if (produced == consumed && produced < num_items) {
			if (previously_producing != 0) {
				printf("Consumer Thread %d is yielding\n", consumer_num);
			}
			previously_producing = 0;
		}
		
		sem_post(&mutex_sem);
		sem_post(&empty_sem);

	}
		
	if (consumed == num_items) {
		printf("Consumer Thread %d is finished\n", consumer_num);
	}

}



int main(int argc, char **argv){
	if (argc != 5){
		printf("Usage: ./prodcon num_producers num_consumers num_buffers num_items\n");
		return 1;
	}

	char *ptr;
	num_producers = strtol(argv[1], &ptr, 10);
	num_consumers = strtol(argv[2], &ptr, 10);
	num_buffers = strtol(argv[3], &ptr, 10);
	num_items = strtol(argv[4], &ptr, 10);
	if(*ptr){	// not empty
		printf("And make those arguments integers\n");
		return 1;
	}
	if (num_producers <= 0 || num_consumers <= 0 || num_buffers <= 0 || num_items <= 0) {
		printf("greater than zero, please!\n");
		return 1;
	}

	sem_init(&empty_sem, 0, num_items);
	sem_init(&full_sem, 0, 0);
	sem_init(&mutex_sem, 0, 1);

	int i;

	// dynamically allocate 2-dimensional buffer array
	buffer = malloc(num_buffers * sizeof(int *));
	for (i = 0; i < num_buffers; i++) {
		buffer[i] = malloc(ITEMS_PER_BUFFER * sizeof(int));
	}

	pthread_t thread_producers[num_producers];
	pthread_t thread_consumers[num_consumers];

	// create producer, consumer threads
	for (i = 0; i < num_producers; i++) {
		pthread_create(&thread_producers[i], 0, producer, (void *)i);
	}
	for (i = 0; i < num_consumers; i++) {
		pthread_create(&thread_consumers[i], 0, consumer, (void *)i);
	}

	// join producer, consumer threads
	for (i = 0; i < num_producers; i++) {
		pthread_join(thread_producers[i], 0);
	}
	for (i = 0; i < num_consumers; i++) {
		pthread_join(thread_consumers[i], 0);
	}
	
	// deallocate 2-dimensional buffer array
	free(buffer);

	sem_destroy(&empty_sem);
	sem_destroy(&full_sem);
	sem_destroy(&mutex_sem);

	return 0;
}