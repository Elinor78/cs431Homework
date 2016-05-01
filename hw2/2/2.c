/*
 * 2.c = Main program
 * Compile 2.c, producer.c, and consumer.c
 * Link -lpthread to producer and consumer
 * Keep all files in the same folder
 */

#include <stdio.h>			// printf
#include <stdlib.h>			// exit
#include <errno.h>			// errno
#include <sys/types.h>		// key_t, sem_t
#include <sys/shm.h>		// shmat, IPC_RMID
#include <semaphore.h>		// sem_open, sem_destroy, sem_wait
#include <fcntl.h>			// 0_CREAT, 0_EXEC

#define MAX 10

int main(void) {
	pid_t producer, consumer, tpid;
	int shmid;     // shared memory id
	int semid;     // semaphore id
	key_t shm_key; // shared memory key
	key_t sem_key; // semaphore key
	char* argv[] = {NULL};

	/* Generate shared memory key */
	shm_key = ftok("shmkey.conf", 1);

	/* Allocate a shared memory segment */
	if ((shmid = shmget(shm_key, MAX * sizeof(int), IPC_CREAT | 0666)) < 0) {
		perror("Allocating shared memory segment failed.");
		exit(1);
	}

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
		sleep(10);

		/* Deallocate the shared memory segment. */
		shmctl(shmid, IPC_RMID, 0);
		
		printf("All processes finished.\n");
	}

	return 0;
}
