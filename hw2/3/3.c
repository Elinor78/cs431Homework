/*
 * 3.c = Main program
 *
 * Instructions:
 * 1) Compile
 * 2) Name 3 whatever you want and link -lpthread
 * 3) Name producer.exe and consumer.exe and link -lpthread
 * (or change name of files below)
 *
 * sharedfile.txt can have anything in it as long as it has <= 10 chars
 * 
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
	pid_t producer, consumer, wpid;
	int status;    // status of child
	int semid;     // semaphore id
	sem_t *sem;    // semaphore
	key_t sem_key; // semaphore key
	char* argv[] = {NULL};

	/* Create semaphore */
	sem = sem_open("semaphoreV", O_CREAT | O_EXCL, 0666, 1);
	sem_unlink("semaphoreV");

	/* Use fork() and exec() to create producer and consumer processes */
	consumer = fork();
	if (consumer == 0) {
		// Child of main
		producer = fork();
		if (producer == 0) {
			// Child of consumer
			execv("producer.exe", argv);
			exit(1);
		} else if (producer > 0) {
			// Parent of producer
			execv("consumer.exe", argv);
			exit(1);
		} else {
			perror("Fork failed\n");
			exit(1);
		}
	} else if (consumer > 0) {
		// Parent of consumer
		printf("Waiting for producer and consumer to finish.\n");
	} else {
		perror("Fork failed\n");
		exit(1);
	}

	/* Wait for child processes to finish */
	while ((wpid = wait(&status)) > 0){
		printf("Exit status of process %d was %d\n", (int)wpid, status);
	}

	printf("All processes finished.\n");

	return 0;
}
