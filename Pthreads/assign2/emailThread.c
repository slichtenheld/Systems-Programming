#include "emailThread.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "calendarADT.h"
#include "macros.h"
#include "mtxGlbl.h"
#include "circBuf.h"

#define gettid() syscall(SYS_gettid)

void printCalEvDebug(struct calendarEvent_t c){
	printf("Type: %c, Title: %s, Date: %s, Time: %s, location: %s\n",c.type, c.calItem.title, c.calItem.date, c.calItem.time, c.calItem.location);
}

int parseEmail(char * buffer, struct calendarEvent_t *c){ // returns -1 if doesn't have everything
	char *pch = strtok (buffer," ");
	unsigned int counter = 0;
	while (pch != NULL){
		
	    switch(counter++) {
	    	case 0: break; //do nothing, ignore "Subject:"
	    	case 1: c->type = pch[0]; break;
	    	case 2: memcpy(c->calItem.title,pch, strlen(pch)+1); break;
	    	case 3: memcpy(c->calItem.date,pch, strlen(pch)+1); break;
	    	case 4: memcpy(c->calItem.time,pch, strlen(pch)+1); break;
	    	case 5: memcpy(c->calItem.location,pch, strlen(pch)-1); break;/* stupid fix for newline character */
	    }
	    pch = strtok (NULL, ",");
	}
	if (counter != 6) return -1; // make sure correct amount of fields
	return 0;
}

void * producer(void * arg) {

	struct circularBuffer *circbuf = (struct circularBuffer*) arg;

	/* initialize buffer for getline */
	char *buffer = NULL; // getline will allocate space for buffer
	int charsRead = 0;
	long unsigned int len = 0; // length of buffer

	/* initialize variables for parsing */
	struct calendarEvent_t	c; // reusable struct for calendarevents

	while(1) {
		//DEBUG_PRINT( ("THREAD P: %ld\n", gettid() ) );

		/* take in input from stdin */
		char charsRead = getline(&buffer,&len,stdin);
		int parsed = 0;

		/* if !EOF, parse input in buffer and put in struct => else set poison pill */
		if ( charsRead != -1 )
			parsed = parseEmail(buffer, &c); 
		else 
			c.type = 'E'; // poison pill still needs to be sent before can exit thread

		/* Buffer written to here */
		if (charsRead == -1 || parsed != -1) {	
			DEBUG_PRINT( ("Type: %c, Title: %s, Date: %s, Time: %s, location: %s\n",c.type, c.calItem.title, c.calItem.date, c.calItem.time, c.calItem.location) );
			/* CRITICAL SECTION BEGIN */ 
			pthread_mutex_lock(&mtx); // acquire lock
			{

				/* give up lock if buffer is full and wait to be woken up */
				while ( circbuf->numItems == circbuf->size ) // while loop because thread could be woken up due to other conditions
					pthread_cond_wait(&prodCond,&mtx);


				memcpy( &(circbuf->buffer[circbuf->in]), &c, sizeof(struct calendarEvent_t) ); // copy struct to buffer
				circbuf->in  = (circbuf->in + 1) % circbuf->size;
				circbuf->numItems++;

				/*** OPTIMIZATION: CONSUMER WILL ONLY BE SLEEPING IF BUFFER WAS EMPTY, AND BUFFER JUST GOT INCREMENTED BY 1 ***/
			 	if (circbuf->numItems == 1) 
					pthread_cond_signal(&consCond);
			
			}
			pthread_mutex_unlock(&mtx);
			/* CRITICAL SECTION END */
			
		}
		if (charsRead == -1)
			break;
	}
	/* Cleanup */
	free(buffer);
	pthread_exit(NULL);
}

