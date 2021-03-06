/*
 * 2.c = Main program
 *
 * Instructions:
 * 1) Download all files to the same folder
 * 2) Run "make" command
 */
 
#include <stdio.h>       // printf
#include <stdlib.h>      // exit
#include <errno.h>       // errno
#include <sys/types.h>   // key_t, sem_t
#include <sys/shm.h>     // shmat, IPC_RMID
#include <semaphore.h>   // sem_open, sem_destroy, sem_wait
#include <fcntl.h>       // 0_CREAT, 0_EXEC

#define MAX 1000

int shmid;   // shared memory id
sem_t *sem;  // semaphore
int* buffer; // buffer in shared memory

int consume() {
	int i;

	/* Do down on semaphore */
	sem_wait(sem);

	/* Consume items in shared memory */
	for (i = 0; i < MAX; i++) {
		/* Consume item */
		if (buffer[i] != 0) {
			buffer[i] = 0;
			printf("Consumed: %d of %d items left.\n", MAX-i-1, MAX);
		} else {
			printf("Consumed nothing. Buffer empty.\n");
			break;
		}
	}

	/* Do up on semaphore */
	sem_post(sem);

	return 0;
}

int main(void) {
	key_t shm_key;      // shared memory key
	key_t sem_key;      // semaphore key
	time_t start, end;  // used as timer
	int terminator = 1; // change to 0 in while loop when done
	double elapsed;     // elapsed time

	/* Generate shared memory key */
	shm_key = ftok("shmkey.conf", 1);

	/* Locate shared memmory segment */
	if ((shmid = shmget(shm_key, MAX * sizeof(int), 0666)) < 0) {
		perror("Can't locate shared memory segment.");
		exit(1);
	}

	/* Attach segment to data space */
	buffer = (int*)shmat(shmid, NULL, 0);

	/* Create semaphore */
	sem = sem_open("semaphore", O_CREAT, 0666, 1);
	sem_unlink("semaphore");

	start = time(NULL);
	while(terminator) {
		end = time(NULL);
		elapsed = end - start;
		if (elapsed < 5) {
			consume();
			sleep(1);
		} else {
			terminator = 0;
		}
	}

	/* Detach from shared memory segment */
	shmdt(buffer);

	/* Deallocate shared memory */
	shmctl(shmid, IPC_RMID, 0);

	return 0;
}
