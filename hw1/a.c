#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define N 3

int buffer[N];
int count;
int num, x;

void producer();
void consumer();

int main(){
	while(TRUE){
		producer();
		consumer();
	}
	return 0;
}

void produce_item(int count){
	num = rand();
	buffer[count] = num;
	printf("Produced %d in buffer slot %d\n", num, count );
}

int consume_item(int count){
	num = buffer[count];
	printf("Consumed %d from buffer slot %d\n", num, count);
	return num;
}

void producer(){
	count = 0;
	while(count < N){
		produce_item(count);
		count++;
	}
}

void consumer(){
	count = 0;
	while(count < N){
		x = consume_item(count);
		count++;
	}
}

