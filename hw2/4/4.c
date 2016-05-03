#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void writer (const char* message, int count, FILE* stream)
{
	for(; count > 0; --count)
    {
       	//Writing to stream (Realize it is sent immediately)
		fprintf(stream, "%s\n", message);
		fflush (stream);
      
       	//Wait a bit to prevent blocking before  writing out more on pipe
      	sleep(1);
    }
}

void reader (FILE* stream)
{
  	char buffer[1024];

   	//Read until you hit end of STREAM!fgets reads until either newline or end-of-file
  	while(!feof(stream) && fgets(buffer, sizeof(buffer), stream) != NULL)
  	{
     	//Copy Stream into buffer
    	fputs(buffer, stdout);

    	if(ferror(stream))
    	{
    		exit(1);
    	}
   	}

}

void consumer(int fd[]){
	FILE* stream;
	
	close(fd[1]);
	stream = fdopen(fd[0], "r");
	reader(stream);
	close(fd[0]);
}

void producer(int fd[]){
	FILE* stream;

	close(fd[0]);
	stream = fdopen(fd[1], "w");
	writer("Hello!", 10, stream);
	close(fd[1]);
}

int main(int argc, char const *argv[])
{
	int file_descriptors[2];
	pid_t pid, wpid;
	int status;

	pipe(file_descriptors);

	pid = fork();

	if(pid == 0)//child 1: consumer
	{
		printf("Consumer child, pid: %d\n", getpid());
		consumer(file_descriptors);
	}
	else//parent
	{
		pid = fork();
		if (pid == 0)//child 2: producer
		{
			printf("Producer Child, pid: %d\n", getpid());
			producer(file_descriptors);

		}
		else//parent
		{
			close(file_descriptors[0]);
			close(file_descriptors[1]);
			while ((wpid = wait(&status)) > 0){
				printf("Exit status of process %d was %d\n", (int)wpid, status);
			}
		}
	}

	return 0;
}