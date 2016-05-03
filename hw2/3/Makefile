CC=gcc
LDFLAGS=-lpthread

all: 3 producer.exe consumer.exe

3: 3.o
	$(CC) -o 3 3.o $(LDFLAGS)

producer.exe: producer.o
	$(CC) -o producer.exe producer.o $(LDFLAGS)

consumer.exe: consumer.o
	$(CC) -o consumer.exe consumer.o $(LDFLAGS)

3.o: 3.c
	$(CC) -c 3.c

producer.o: producer.c
	$(CC) -c producer.c

consumer.o: consumer.c
	$(CC) -c consumer.c

clean: 
	rm -f *.o 3 producer.exe consumer.exe