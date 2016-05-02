CC=gcc
LDFLAGS=-lpthread

all: 2 producer.exe consumer.exe

2: 2.o
	$(CC) -o 2 2.o $(LDFLAGS)

producer.exe: producer.o
	$(CC) -o producer.exe producer.o $(LDFLAGS)

consumer.exe: consumer.o
	$(CC) -o consumer.exe consumer.o $(LDFLAGS)

2.o: 2.c
	$(CC) -c 2.c

producer.o: producer.c
	$(CC) -c producer.c

consumer.o: consumer.c
	$(CC) -c consumer.c

clean: rm -f *.o 2 producer.exe consumer.exe