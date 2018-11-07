CC = gcc
CFLAGS = -c -g -Wall

.SUFFIXES: .o .c
.c.o:

	$(CC)  $(CFLAGS)  $*.c

EXECS = clientTCP  serverTCP

all: $(EXECS)

serverTCP: serverTCP.o
	$(CC)  serverTCP.o  -o $@

clientTCP: clientTCP.o 
	$(CC)  clientTCP.o -o $@

clean:
	rm -f  *.o
	rm -f $(EXECS)
