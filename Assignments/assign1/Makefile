CC = gcc


all: 	calendarFilter emailFilter parent

emailFilter: emailfilter.c 
			$(CC) emailfilter.c -o emailfilter

calendarFilter: calendarfilter.c 
			$(CC) calendarfilter.c calendarADT.c -o calendarfilter

parent: locationupdater.c 
			$(CC) locationupdater.c -o locationupdater

clean: 	
		-rm -rf emailfilter calendarfilter locationupdater