CC = gcc
CFLAGS = -Wall -g

OBJECTS = circBuf.o linkedList.o transfProg.o workerThread.o structs.o
DEPS = circBuf/fifo.h linkedlist/list.h structs.h workerThread.h

all: transfProg

profile: CFLAGS += -pg
profile: transfProg

transfProg: $(OBJECTS)
	$(CC) $(CFLAGS) -o transfProg $(OBJECTS) -pthread

zip:
	zip Lichtenheld_Samuel_Assignment3.zip *.c *.h Makefile *.txt inputGenerator/inputGenerator.c inputGenerator/*.txt

transfProg.o : transfProg.c $(DEPS)
	$(CC) $(CFLAGS) -c transfProg.c

circBuf.o : circBuf/circBuf.c circBuf/fifo.h
	$(CC) $(CFLAGS) -c circBuf/circBuf.c

linkedList.o : linkedlist/linkedList.c linkedlist/list.h
	$(CC) $(CFLAGS) -c linkedlist/linkedList.c

workerThread.o : workerThread.c workerThread.h
	$(CC) $(CFLAGS) -c workerThread.c

structs.o : structs.c structs.h
	$(CC) $(CFLAGS) -c structs.c
	
clean: 
	rm transfProg Lichtenheld_Samuel_Assignment3.zip *.o