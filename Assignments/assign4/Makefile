
CC = gcc

OBJECTS = transfProg.o
DEPS = macros.h structs.h

all: transfProg

debug:	CC += -DDEBUG
debug:	transfProg

transfProg : $(DEPS) $(OBJECTS)
	$(CC) -o transfProg $(OBJECTS) -pthread

zip:
	zip Lichtenheld_Samuel_Assignment4.zip *.c *.h Makefile *.txt

rmzip: 
	rm Lichtenheld_Samuel_Assignment4.zip



transfProg.o : transfProg.c $(DEPS)
	$(CC) -c transfProg.c

# main.o : main.c $(DEPS)
# 	$(CC) -c main.c

# calThread.o : calThread.c $(DEPS)
# 	$(CC) -c calThread.c

# emailThread.o : emailThread.c $(DEPS)
# 	$(CC) -c emailThread.c

# calendarADT.o : calendarADT.c $(DEPS)
# 	$(CC) -c calendarADT.c

clean:
	rm transfProg $(OBJECTS)

