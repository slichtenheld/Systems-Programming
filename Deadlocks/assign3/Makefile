CC = gcc

OBJECTS = circBuf.o transfProg.o
DEPS = circBuf/fifo.h linkedlist/list.h structs.h

all: transfProg

transfProg: $(OBJECTS)
	$(CC) -o transfProg $(OBJECTS) -pthread

zip:
	zip Lichtenheld_Samuel_Assignment3.zip *.c *.h Makefile *.txt

transfProg.o : transfProg.c $(DEPS)
	$(CC) -c transfProg.c

circBuf.o : circBuf/circBuf.c circBuf/fifo.h
	$(CC) -c circBuf/circBuf.c

clean: 
	rm transfProg Lichtenheld_Samuel_Assignment3.zip *.o