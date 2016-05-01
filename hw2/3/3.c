/*
 * 3.c = Main program
 * Compile and link -lpthread
 * Keep all files in the same folder
 */

#include <stdio.h>       // printf
#include <stdlib.h>      // exit
#include <errno.h>       // errno
#include <sys/types.h>   // key_t, sem_t
#include <semaphore.h>   // sem_open, sem_destroy, sem_wait
#include <fcntl.h>       // 0_CREAT, 0_EXEC

#define MAX 10

int main(void) {
	pid_t producer, consumer, tpid;
	int semid;     // semaphore id
	sem_t *sem;    // semaphore
	key_t sem_key; // semaphore key
	char* argv[] = {NULL};

	/* Create semaphore */
	sem = sem_open("semaphoreV", O_CREAT | O_EXCL, 0666, 1);
	sem_unlink("semaphoreV");

	/* Use fork() and exec() to create producer and consumer processes */
	if (consumer = fork()) {
		if (producer = fork()) {
			execv("producer.exe", argv);
			exit(0);
		}
		execv("consumer.exe", argv);
		exit(0);
	} else {
		printf("Waiting for producer and consumer to finish.\n");
		sleep(15);
		
		printf("All processes finished.\n");
	}

	return 0;
}
