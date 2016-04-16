#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t condition_consumer, condition_producer;
int buffer = 0;

struct ptr_struct
{
	int M;
};


void *producer(void *ptr){
	struct ptr_struct *p = (struct ptr_struct *)ptr;
	int i;
	for(i = 0; i < p->M; i++){
		pthread_mutex_lock(&mutex);
		while(buffer !=0){
			pthread_cond_wait(&condition_producer, &mutex);
		}
		buffer = rand();
		printf("Produced %d in the buffer\n", buffer);
		pthread_cond_signal(&condition_consumer);
		pthread_mutex_unlock(&mutex);
	}

} 
void *consumer(void *ptr){
	struct ptr_struct *p = (struct ptr_struct *)ptr;
	int i;
	for(i = 0; i < p->M; i++){
		pthread_mutex_lock(&mutex);
		while(buffer == 0){
			pthread_cond_wait(&condition_consumer, &mutex);
		}
		printf("Consumed %d from the buffer\n", buffer);
		buffer = 0;
		pthread_cond_signal(&condition_producer);
		pthread_mutex_unlock(&mutex);
	}
}

int main(int argc, char **argv){
	if (argc != 2){
		printf("Only pass one argument for number of times to run producer-consumer\n");
		return 1;
	}
	char *ptr;
	long N = strtol(argv[1], &ptr, 10);
	if(*ptr){
		printf("And make that argument an integer\n");
		return 1;
	}
	if (N <= 0){
		printf("greater than zero, please!\n");
		return 1;
	}

	struct ptr_struct p;
	p.M = N;
	pthread_t prod_thread, cons_thread;
	pthread_mutex_init(&mutex, 0);
	pthread_cond_init(&condition_consumer, 0);
	pthread_cond_init(&condition_producer, 0);
	pthread_create(&cons_thread, 0, consumer, (void *)&p);
	pthread_create(&prod_thread, 0, producer, (void *)&p);
	pthread_join(prod_thread, 0);
	pthread_join(cons_thread, 0);
	pthread_cond_destroy(&condition_consumer);
	pthread_cond_destroy(&condition_producer);
	pthread_mutex_destroy(&mutex);

	return 0;
}