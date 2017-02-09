
CC = gcc

OBJECTS = main.o calThread.o emailThread.o calendarADT.o
DEPS = macros.h emailThread.h calThread.h circBuf.h mtxGlbl.h calendarADT.h

all: assign2

debug:	CC += -DDEBUG
debug:	assign2

assign2 : $(OBJECTS)
	$(CC) -o assign2 $(OBJECTS) -pthread

zip:
	zip Lichtenheld_Samuel_Assignment2.zip *.c *.h Makefile README.txt

rmzip: 
	rm Lichtenheld_Samuel_Assignment2.zip




main.o : main.c $(DEPS)
	$(CC) -c main.c

calThread.o : calThread.c $(DEPS)
	$(CC) -c calThread.c

emailThread.o : emailThread.c $(DEPS)
	$(CC) -c emailThread.c

calendarADT.o : calendarADT.c $(DEPS)
	$(CC) -c calendarADT.c

clean:
	rm assign2 $(OBJECTS)

