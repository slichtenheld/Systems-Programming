#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calendarEvent.h"

int parseEmail(char * buffer, struct calendarEvent_t *c);

int main(int argc, char *argv[]){
	
	// initialize variables
	char *buffer = NULL; // getline will allocate space for buffer
	int charsRead = 0;
	long unsigned int len = 0; // length of buffer

	struct calendarEvent_t c;//
	
	while(charsRead = getline(&buffer,&len,stdin) > 0 ){ // read in line till EoF
		if (parseEmail(buffer, &c) != -1) printCalendarEvent(c);
	}

	free(buffer); // need to free buffer allocated by getline!!!!
	return(0);
}



int parseEmail(char * buffer, struct calendarEvent_t *c){ // returns -1 if doesn't have everything
	char *pch = strtok (buffer," ");
	unsigned int counter = 0;
	while (pch != NULL){
		
	    switch(counter++) {
	    	case 0: break; //do nothing, ignore "Subject:"
	    	case 1: c->type = pch[0]; break;
	    	case 2: memcpy(c->title,pch, strlen(pch)+1); break;
	    	case 3: memcpy(c->date,pch, strlen(pch)+1); break;
	    	case 4: memcpy(c->time,pch, strlen(pch)+1); break;
	    	case 5: 
	    		memcpy(c->location,pch, strlen(pch)-1); /* stupid fix for newline character */
	    		break;
	    }
	    pch = strtok (NULL, ",");
	    
	}

	if (counter != 6) return -1;
	return 0;
}