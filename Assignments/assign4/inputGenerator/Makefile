CC = gcc

OBJECTS = inputGenerator.o 

all: inputGenerator

inputGenerator : $(OBJECTS)
	$(CC) -o inputGenerator $(OBJECTS)

inputGenerator.o : inputGenerator.c 
	$(CC) -c inputGenerator.c