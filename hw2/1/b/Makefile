bproc: b.o
	gcc $(CFLAGS) -o bproc b.o -lpthread
b.o: b.c
	gcc $(CFLAGS) -c b.c
clean:
	rm -f *.o bproc
