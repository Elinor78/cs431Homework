#include <pthread.h>
#include <stdio.h>

#define N 100		// number of slots in the buffer
#define TRUE 1
#define FALSE 0

int count = 0;

int produce_item() {
	printf("Produced item: %d\n", count);
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

void consumer() {
	int item;
	
	while (TRUE) {
		if (count == 0) {	// if buffer is empty, return to producer
			return;
		}
		item = remove_item();	// remove item from buffer
		count = count - 1;			// decrement count of items in buffer
		if (count == N - 1) {		// return to producer if buffer size == 99
			return;
		}
		consume_item(item);		// print item
	}
	return;
}

void producer() {
	int item;
	
	while (TRUE) {
		item = produce_item();	// generate next item
		if (count == N) {			// call consumer if buffer is full
			consumer();	
		}
		insert_item(item);		// put item in buffer
		count = count + 1;			// increment count of items in buffer
		if (count == 1) {			// call consumer if buffer size == 1
			consumer();
		}
	}
	return;
}


int main() {
  	//pthread_t thread_id;
	//pthread_create (&thread_id, NULL, &producer, NULL);

	producer();
    	
	//while(1)
	//fputs("", stderr);
	return 0;
}


