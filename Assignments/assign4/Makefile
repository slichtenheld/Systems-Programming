
CC = gcc

OBJECTS = transfProg.o circBuf.o structs.o
DEPS = macros.h structs.h circBuf/fifo.h structs.h

all: transfProg

debug:	CC += -DDEBUG
debug:	transfProg

transfProg : $(OBJECTS)
	$(CC) -o transfProg $(OBJECTS) -pthread

zip:
	zip Lichtenheld_Samuel_Assignment4.zip *.c *.h Makefile *.txt

rmzip: 
	rm Lichtenheld_Samuel_Assignment4.zip



transfProg.o : transfProg.c $(DEPS)
	$(CC) -c transfProg.c

circBuf.o : circBuf/circBuf.c circBuf/fifo.h
	$(CC) -c circBuf/circBuf.c

structs.o : structs.c structs.h
	$(CC) -c structs.c

clean:
	rm transfProg $(OBJECTS)

