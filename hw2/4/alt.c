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
  while(fgets(buffer, sizeof(buffer), stream) != NULL)
  {
     //Copy Stream into buffer
    fputs(buffer, stdout);
    if (feof(stream)){
    	exit(0);
    }
   }

}
void producer(int fd[])
{
	pid_t pid;

	if ((pid = fork()) == -1){
		perror("fork");
		exit(1);
	}
	if (pid == 0){
		//producer process
		printf("Child 2 %d\n", getpid());
		FILE* stream;
		close(fd[0]);

		stream = fdopen(fd[1], "w");
		writer("Hello!", 5, stream);
		close(fd[1]);
		exit(0);
	}
	//parent does nothing
}

void consumer(int fd[])
{
	pid_t pid;

	if ((pid = fork()) == -1){
		perror("fork");
		exit(1);
	}
	if (pid == 0){
		//consumer process
		printf("Child 1 %d\n", getpid());
		FILE* stream;
		close(fd[1]);

		stream = fdopen(fd[0], "r");
		reader(stream);
		close(fd[0]);
		exit(0);
	}
	//parent does nothing
}

int main(int argc, char const *argv[])
{
	int file_descriptors[2];
	pid_t wpid;
	int status;

	pipe(file_descriptors);
	consumer(file_descriptors);
	producer(file_descriptors);
	close(file_descriptors[0]);
	close(file_descriptors[1]);


	while ((wpid = wait(&status)) > 0){
		printf("Exit status of process %d was %d\n", (int)wpid, status);
	}

	return 0;
}