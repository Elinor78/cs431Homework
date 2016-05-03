
/*
Showing example of SOCKETS on LOCAL HOST - Nima's code
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/*
  Read text from local namespace socket and print it out. Continue until Socket Closes.
  Return nonzero if the client sent a "quit" message, zero otherwise
*/

int server(int client_socket)
{
    while(1)
    {
        int length;
        char* text;
        //First read the length of the text message from the socket. If read = 0 Client closed connection
        if(read(client_socket, &length, sizeof(length)) == 0)
            return 0;

        //Dyncamically allocate a buffer in heap space to hold the text of size length
        text = (char*) malloc (length);

        //Read from socket and store in text buffer and print out
        read (client_socket, text, length);
        printf("%s\n", text);

        //String compare to see if "quit" was sent, if so break out of while loop
        if (!strcmp (text, "quit"))
        {
            free(text);
            return 1;
        }

        //Deallocate buffer from heap [no garbage collector]
        free (text);
    }
}

int main(int argc, char* const argv[])
{
    printf("Server Process, pid: %d\n", getpid());
    const char* const socket_name = argv[1];
    int socket_fd;     //like a file descriptor used as handle to socket
    struct sockaddr_un name;
    int client_sent_quit_message;


     //Create Socket - establish phone
    socket_fd = socket (PF_LOCAL, SOCK_STREAM, 0);    //Using Local namespace,SOCK_STREAM-Connection style

     //Indicate this is the server and get unique address -- Receiver in the phone system and get phone #
    name.sun_family = AF_LOCAL;
    strcpy (name.sun_path, socket_name);
    bind (socket_fd, (struct sockaddr *)&name, SUN_LEN (&name));

     //Listen for Connection - Turn on Ringer and wait for call
    listen (socket_fd, 5);

     //Always stay in Listen mode Accept calls by spinning off one server to deal with each client  
    // Break out of loop if a client sends "quit" message
    do
    {
        struct sockaddr_un client_name;
        socklen_t client_name_len = sizeof(struct sockaddr_un);
        int client_socket_fd;

        //Accept a connection - pick up phone
        client_socket_fd = accept (socket_fd, (struct sockaddr *)&client_name, &client_name_len);

        //Sping off new server and set quit flag if client sends "quit" 
        client_sent_quit_message = server (client_socket_fd);

        //Close connection -  HANG UP
        close(client_socket_fd);
    }
    while (!client_sent_quit_message);

    //Remove socket file and unlink program deleting file so other processes can access it
    close (socket_fd);
    unlink (socket_name);

    return 0;
}


