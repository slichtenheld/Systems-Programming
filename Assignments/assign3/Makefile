CC = gcc

OBJECTS = circBuf.o linkedList.o transfProg.o workerThread.o structs.o
DEPS = circBuf/fifo.h linkedlist/list.h structs.h workerThread.h

all: transfProg

transfProg: $(OBJECTS)
	$(CC) -o transfProg $(OBJECTS) -pthread

zip:
	zip Lichtenheld_Samuel_Assignment3.zip *.c *.h Makefile *.txt

transfProg.o : transfProg.c $(DEPS)
	$(CC) -c transfProg.c

circBuf.o : circBuf/circBuf.c circBuf/fifo.h
	$(CC) -c circBuf/circBuf.c

linkedList.o : linkedlist/linkedList.c linkedlist/list.h
	$(CC) -c linkedlist/linkedList.c

workerThread.o : workerThread.c workerThread.h
	$(CC) -c workerThread.c

structs.o : structs.c structs.h
	$(CC) -c structs.c
	
clean: 
	rm transfProg Lichtenheld_Samuel_Assignment3.zip *.o