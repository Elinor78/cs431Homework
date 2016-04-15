cproc: c.o
	gcc $(CFLAGS) -o cproc c.o -lpthread

c.o: c.c
	gcc $(CFLAGS) -c c.c

clean:
	rm -f *.o cproc
