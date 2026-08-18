CC = gcc
CFLAGS = -Wall -Wextra

program: main.o function1.o function2.o function3.o
	$(CC) main.o function1.o function2.o function3.o -o program

main.o: main.c functions.h helpers.h
	$(CC) $(CFLAGS) -c main.c

function1.o: function1.c functions.h helpers.h
	$(CC) $(CFLAGS) -c function1.c

function2.o: function2.c functions.h helpers.h
	$(CC) $(CFLAGS) -c function2.c

function3.o: function3.c functions.h helpers.h
	$(CC) $(CFLAGS) -c function3.c

clean:
	rm -f *.o program