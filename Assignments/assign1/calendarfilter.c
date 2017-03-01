#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calendarADT.h"

//read in line from stdin, returns -1 if misread input
int readInput(char** buffer);

//parse input and return event, returns C, D, X or NULL
char parseInput(char *buffer, struct CalendarItem_t *c);

int main(int argc, char *argv[]){
	
	/* initialize data structure */
	Calendar_T cal = Calendar_new();
	
	/* TODO: implement later */
	// set function pointers to work for specific data structure
	// int (*addCalEvent)(struct calendarEvent c); 
	// int (*delCalEvent)(struct calendarEvent c);
	// int (*modCalEvent)(struct calendarEvent c);


	// reusing buffer and event throughout process
	char *buffer = NULL; // getline will allocate space for buffer
	long unsigned int len; 
	

	while(readInput(&buffer) != -1){ // reads in line from stdin
			struct CalendarItem_t *c = malloc(sizeof *c);
		 	switch(parseInput(buffer, c)){
				case 'C': /* add event to data structure */
					Calendar_add(cal, c);
					break; 
				case 'D': /* delete event from data structure */
					Calendar_del(cal, c);
					break; 
				case 'X': /* modify event in data structure */
					Calendar_mod(cal, c);
					break; 
					/* 
					print_calItem(*c);
					printf("- - - - - - - - - - - -\n");
					Calendar_print(cal);
					printf("_ _ _ _ _ _ _ _ _ _ _ _ _\n");
					*/ 
				default: printf("incompatible calendarEvent type\n");
			}
	}

	free(buffer); // need to free buffer allocated by getline!!!!
	//Calendar_print(cal);
	return(0);
}

int readInput(char** buffer){
	long unsigned int len; // length of buffer
	return getline(buffer,&len,stdin);
}

char parseInput(char *buffer, struct CalendarItem_t *c){
	char temp;
	char *pch = strtok (buffer," ,");
	unsigned int counter = 0;
  		while (pch != NULL){
		    switch(++counter) {
		    	case 1: temp = pch[0]; break;
		    	case 2: memcpy(c->title,pch, strlen(pch)+1); break;
		    	case 3: memcpy(c->date,pch, strlen(pch)+1); break;
		    	case 4: memcpy(c->time,pch, strlen(pch)+1); break;
		    	case 5: memcpy(c->location,pch, strlen(pch)+1); break;
		    	//case 6: printf("Case 6 triggered\n"); break;
		    	//default: perror("error parseinput");
		    }
		    pch = strtok (NULL, " ,");
		}
	if (counter < 6) return 0;
	return temp;
}

// some data structure needed
	// will use array for now, but code should be able to
	// easily transition to a more fitting data structure

// read in line
	// if C (Create)
		// add new event to data structure
	// else if D (Delete)
		// find event in data structure with same title,date,and time and delete
	// else if X (Modify)
		// find event in data structure with same title and date, modify with new provided time and location