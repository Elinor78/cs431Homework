all: consumer producer 5proc

consumer: socket-server.o
	gcc $(CFLAGS) -o consumer socket-server.o 

socket-server.o: socket-server.c
	gcc $(CFLAGS) -c socket-server.c

producer: socket-client1.o
	gcc $(CFLAGS) -o producer socket-client1.o 

socket-client1.o: socket-client1.c
	gcc $(CFLAGS) -c socket-client1.c

5proc: 5.o
	gcc $(CFLAGS) -o 5proc 5.o 

5.o: 5.c
	gcc $(CFLAGS) -c 5.c

clean:
	rm -f *.o consumer producer 5proc