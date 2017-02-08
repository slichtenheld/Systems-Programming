
objects = main.o calThread.o emailThread.o calendarADT.o
alldependencies = macros.h emailThread.h calThread.h circBuf.h mtxGlbl.h calendarADT.h

assign2Make : $(objects)
	cc -o assign2Make $(objects) -pthread

debug: $(objects)
	cc -o assign2Make $(objects) -pthread -D DEBUG

zip:
	zip Lichtenheld_Samuel_Assignment2.zip *.c *.h Makefile README.txt

rmzip: 
	rm Lichtenheld_Samuel_Assignment2.zip




main.o : main.c $(alldependencies)
	cc -c main.c

calThread.o : calThread.c $(alldependencies)
	cc -c calThread.c

emailThread.o : emailThread.c $(alldependencies)
	cc -c emailThread.c

calendarADT.o : calendarADT.c $(alldependencies)
	cc -c calendarADT.c

clean:
	rm assign2Make $(objects)

