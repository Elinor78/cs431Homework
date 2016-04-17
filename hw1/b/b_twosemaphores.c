#include <pthread.h> // for pthread_create, pthread_join, pthread_exit
#include <stdio.h>   // for printf 
#include <stdlib.h>  // for exit
#include <semaphore.h>

#define N 100
sem_t empty;
sem_t full;
int buffer[N];

void check_item(int buffer_item, int item){
	if (buffer_item != item){
		printf("Thread issue\n");
		exit(1);
	}
}

void produce_item(int i){
	buffer[i] = i;
	printf("put %d in buffer slot %d\n",buffer[i], i);
	check_item(buffer[i], i);
}

int consume_item(int i){
	printf("got %d from buffer slot %d\n", buffer[i], i);
	check_item(buffer[i], i);
	int tmp = buffer[i];
	buffer[i] = 0;
	return tmp;
}

void *producer(void *ptr){
	int i;
	for(i = 0; i < N; i++){
		sem_wait(&empty);
		produce_item(i);
		sem_post(&full);
	}
}

void *consumer(void *ptr){
	int i;
	for(i = 0; i < N; i++){
		sem_wait(&full);
		int temp = consume_item(i);
		sem_post(&empty);
	}
}

int main(int argc, char const *argv[])
{
	sem_init(&empty, 0, N);
	sem_init(&full, 0, 0);

	pthread_t prod_thread, cons_thread;
	pthread_create(&cons_thread, 0, consumer, 0);
	pthread_create(&prod_thread, 0, producer, 0);
	pthread_join(prod_thread, 0);
	pthread_join(cons_thread, 0);

	return 0;
}