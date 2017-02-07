
objects = main.o calThread.o emailThread.o calendarADT.o


assign2Make : $(objects)
	cc -o assign2Make $(objects) -pthread

debug: $(objects)
	cc -o assign2Make $(objects) -pthread -D DEBUG

zip:
	zip Lichtenheld_Samuel_Assignment1.zip *.c *.h Makefile README.txt

rmzip: 
	rm Lichtenheld_Samuel_Assignment1.zip




main.o : main.c macros.h emailThread.h calThread.h circBuf.h mtxGlbl.h
	cc -c main.c

calThread.o : calThread.c calThread.h macros.h calendarADT.h mtxGlbl.h circBuf.h
	cc -c calThread.c

emailThread.o : emailThread.c emailThread.h calendarADT.h macros.h mtxGlbl.h circBuf.h
	cc -c emailThread.c

calendarADT.o : calendarADT.c calendarADT.h
	cc -c calendarADT.c

clean:
	rm assign2Make $(objects)

