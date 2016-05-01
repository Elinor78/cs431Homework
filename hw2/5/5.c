/*
    Combined the (modified) spawn example (fifth.c) with the socket server and client examples
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int spawn( char* program, char** arg_list)
{
    pid_t child_pid;  //PID = 16 bit value 

    if((child_pid = fork()) < 0){
        perror("fork");
        exit(1);
    }
    if (child_pid > 0)  //parent process
        return child_pid;

    else   //child process
    {
        execvp(program, arg_list);     
        //will only return if there is an error
        fprintf(stderr, "An error occured in execvp\n");
        abort(); 
    }
}

int main(int argc, char const *argv[])
{
    int status;
    pid_t wpid;

    if (argc != 2){
        fprintf(stderr, "Need two arguments\n");
        exit(1);
    }   

    char* arg_list_1[] = { "./consumer", argv[1], NULL};
    char* arg_list_2[] = {"./producer", argv[1], "Hello, this is the message from the client/producer", NULL};

    spawn("./consumer", arg_list_1);

    sleep(1);

    spawn("./producer", arg_list_2);

    while ((wpid = wait(&status)) > 0){
        printf("Exit status of process %d was %d\n", (int)wpid, status);
    }   

    return 0;
}