#include <stdio.h>
#include <stdlib.h>

#define TRUE 1

int *buffer;
int count;
int num, x;


void produce_item(int count){
	num = rand();
	buffer[count] = num;
	printf("Produced %d in buffer slot %d\n", num, count);
}

int consume_item(int count){
	num = buffer[count];
	printf("Consumed %d from buffer slot %d\n", num, count);
	return num;
}

void producer(int N){
	count = 0;
	while(count < N){
		produce_item(count);
		count++;
	}
}

void consumer(int N){
	count = 0;
	while(count < N){
		x = consume_item(count);
		count++;
	}
}

int main(int argc, char const *argv[])
{
	int N;
	if (argc != 2){
		N = 100;
	}else{
		N = atoi(argv[1]);
	}

	buffer = malloc(N * sizeof(int));

	producer(N);
	consumer(N);

	free(buffer);
	return 0;
}

