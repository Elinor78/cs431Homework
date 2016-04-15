#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 10
pthread_mutex_t mutex;
pthread_cond_t condition_consumer, condition_producer;
int buffer = 0;

void *producer(void *ptr){
	int i;
	for(i=1; i <= N; i++){
		pthread_mutex_lock(&mutex);
		while(buffer !=0){
			pthread_cond_wait(&condition_producer, &mutex);
		}
		buffer = rand();
		printf("Produced %d in the buffer, index: %d.\n", buffer, i);
		pthread_cond_signal(&condition_consumer);
		pthread_mutex_unlock(&mutex);
	}

} 
void *consumer(void *ptr){
	int i;
	for(i = 1; i <= N; i++){
		pthread_mutex_lock(&mutex);
		while(buffer == 0){
			pthread_cond_wait(&condition_consumer, &mutex);
		}
		printf("Consuming %d from the buffer:, index %d\n", buffer, i);
		buffer = 0;
		pthread_cond_signal(&condition_producer);
		pthread_mutex_unlock(&mutex);
	}
}

int main(int argc, char **argv){
	pthread_t prod_thread, cons_thread;
	pthread_mutex_init(&mutex, 0);
	pthread_cond_init(&condition_consumer, 0);
	pthread_cond_init(&condition_producer, 0);
	pthread_create(&cons_thread, 0, consumer, 0);
	pthread_create(&prod_thread, 0, producer, 0);
	pthread_join(prod_thread, 0);
	pthread_join(cons_thread, 0);
	pthread_cond_destroy(&condition_consumer);
	pthread_cond_destroy(&condition_producer);
	pthread_mutex_destroy(&mutex);

	return 0;
}