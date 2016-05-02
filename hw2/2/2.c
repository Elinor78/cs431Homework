/*
 * 2.c = Main program
 *
 * Instructions:
 * 1) Compile
 * 2) Name 2 whatever you want and link -lpthread
 * 3) Name producer.exe and consumer.exe and link -lpthread
 * (or change name of files below)
 * 
 * Keep all files in the same folder
 */

#include <stdio.h>       // printf
#include <stdlib.h>      // exit
#include <errno.h>       // errno
#include <sys/types.h>   // key_t, sem_t
#include <sys/shm.h>     // shmat, IPC_RMID
#include <semaphore.h>   // sem_open, sem_destroy, sem_wait
#include <fcntl.h>       // 0_CREAT, 0_EXEC

#define MAX 10

int main(void) {
	pid_t producer, consumer, wpid;
	int status;    // status of child
	int shmid;     // shared memory id
	int semid;     // semaphore id
	key_t shm_key; // shared memory key
	key_t sem_key; // semaphore key
	sem_t *sem;    // semaphore
	char* argv[] = {NULL};

	/* Generate shared memory key */
	shm_key = ftok("shmkey.conf", 1);

	/* Allocate a shared memory segment */
	if ((shmid = shmget(shm_key, MAX * sizeof(int), IPC_CREAT | 0666)) < 0) {
		perror("Allocating shared memory segment failed.");
		exit(1);
	}

	/* Create semaphore */
	sem = sem_open("semaphore", O_CREAT | O_EXCL, 0666, 1);
	sem_unlink("semaphore");

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

	/* Deallocate the shared memory segment. */
	shmctl(shmid, IPC_RMID, 0);
	
	printf("All processes finished.\n");

	return 0;
}
