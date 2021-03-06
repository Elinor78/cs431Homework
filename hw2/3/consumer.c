/*
 * 3.c = Main program
 *
 * Instructions:
 * 1) Download all files to the same folder
 * 2) Run "make" command
 *
 * sharedfile.txt can have anything in it as long as it has >= 10 chars
 */
 
#include <stdio.h>       // printf
#include <stdlib.h>      // exit
#include <errno.h>       // errno
#include <fcntl.h>       // 0_CREAT, 0_EXEC
#include <sys/mman.h>    // mmap, etc
#include <sys/types.h>   // key_t, sem_t
#include <semaphore.h>   // sem_open, sem_destroy, sem_wait
#include <sys/stat.h>    // struct st

#define MAX 10

sem_t *sem;  // semaphore
char *map;   // memory mapped file

int consume() {
	int i;

	/* Do down on semaphore */
	sem_wait(sem);

	/* Consume items in shared memory */
	for (i = 0; i < MAX; i++) {
		/* Consume item */
		if (map[i] != '0') {
			map[i] = '0';
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
	key_t sem_key;      // semaphore key
	struct stat st;     // struct to file size
	int fd;             // for opening files
	time_t start, end;  // used as timer
	int terminator = 1; // change to 0 in while loop when done
	double elapsed;     // elapsed time

	/* Create semaphore */
	sem = sem_open("semaphoreV", O_CREAT, 0666, 1);
	sem_unlink("semaphoreV");

	/* Open file for reading/writing */
	fd = open("sharedfile.txt", O_RDWR, (mode_t)0666);
	if (fd == -1) {
		perror("Could not open file.\n");
		exit(1);
	}

	/* Get file stats */
	if (stat("sharedfile.txt", &st) == -1) {
		perror("Could not get stats.\n");
		exit(1);
	}

	/* Map file */
	map = mmap(0, st.st_size, PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0);
	if (map == MAP_FAILED) {
		close(fd);
		perror("Couldn't map file.\n");
		exit(1);
	}

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

	/* Sync file changes */
	if (msync(map, st.st_size, MS_SYNC) < 0) {
		perror("Can't sync file.\n");
	}

	/* Deallocate mapped memory */
	if (munmap(map, st.st_size) == -1) {
		perror("Can't unmap the file.\n");
	}

	/* Close file */
	close(fd);

	return 0;
}
