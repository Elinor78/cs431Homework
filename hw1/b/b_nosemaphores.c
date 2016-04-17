#include <pthread.h> // for pthread_create, pthread_join, pthread_exit
#include <stdio.h>   // for printf 
#include <stdlib.h>  // for exit
#include <semaphore.h>

#define N 100
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
		produce_item(i);
	}
}

void *consumer(void *ptr){
	int i;
	for(i = 0; i < N; i++){
		int temp = consume_item(i);
	}
}

int main(int argc, char const *argv[])
{

	pthread_t prod_thread, cons_thread;
	pthread_create(&cons_thread, 0, consumer, 0);
	pthread_create(&prod_thread, 0, producer, 0);
	pthread_join(prod_thread, 0);
	pthread_join(cons_thread, 0);

	return 0;
}