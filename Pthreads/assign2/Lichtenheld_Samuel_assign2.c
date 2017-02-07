
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "calendarADT.h"

#define gettid() syscall(SYS_gettid)

// TODO: convert print statements to debug statements


/* global variables, is this the best way? */
pthread_mutex_t mtx; // declare global mutex
pthread_cond_t prodCond,consCond ; //declare global conditions
pthread_cond_t termCond; // termination condition

int counter = 0;
#define COUNTMAX 20
#define PRODTHREADS 1
#define CONSTHREADS 1

struct calendarEvent_t {
	char type;
	struct CalendarItem_t calItem;
};


void printCalEvDebug(struct calendarEvent_t c){
	printf("Type: %c, Title: %s, Date: %s, Time: %s, location: %s\n",c.type, c.calItem.title, c.calItem.date, c.calItem.time, c.calItem.location);
}


struct circularBuffer{
	int size;
	int in;
	int out;
	int numItems;
	struct calendarEvent_t buffer[];
}; 

int parseEmail(char * buffer, struct calendarEvent_t *c);


//read in line from stdin, returns -1 if misread input
int readInput(char** buffer);

//parse input and return event, returns C, D, X or NULL
char parseInput(char *buffer, struct CalendarItem_t *c);

void * producer(void * arg) {

	struct circularBuffer *circbuf = (struct circularBuffer*) arg;

	/* initialize buffer for getline */
	char *buffer = NULL; // getline will allocate space for buffer
	int charsRead = 0;
	long unsigned int len = 0; // length of buffer

	/* initialize variables for parsing */
	struct calendarEvent_t	c; // reusable struct for calendarevents

	while(1) {
		printf("THREAD P: %ld\n", gettid() );

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

void * consumer(void * arg) {
	struct circularBuffer *circbuf = (struct circularBuffer*) arg;

	/* loop until poison pill received */
	while(1) { 
		printf("THREAD C: %ld\n", gettid() );

		struct CalendarItem_t *c = malloc(sizeof *c);
		/* CRITICAL SECTION */
		pthread_mutex_lock(&mtx); // acquire lock
		{
			/* give up lock if buffer is full and wait to be woken up */
			while ( circbuf->numItems == 0 ) // while loop because thread could be woken up due to other conditions
				pthread_cond_wait(&consCond,&mtx);

			



			printCalEvDebug(circbuf->buffer[circbuf->out]);
			//struct CalendarItem_t *c = malloc(sizeof *c);
			circbuf->out = (circbuf->out + 1) % circbuf->size;
			circbuf->numItems --;

			/*** OPTIMIZATION: PRODUCER WILL ONlY BE SLEEPING IF BUFFER WAS FULL, AND NUMITEMS JUST GOT DECREMENTED BY 1 ***/ 
			if (circbuf->numItems == circbuf->size - 1) 
				pthread_cond_signal(&prodCond);
		}
		pthread_mutex_unlock(&mtx);
		/* END CRITICAL SECTION */

		if (circbuf->buffer[circbuf->out].type == 'E') {
				pthread_mutex_unlock(&mtx);
				break; //TODO: remove from critical section
		}
		switch (circbuf->buffer[circbuf->out].type){
				case 'E': break;
				case 'C': break;
				case 'D': break;
				case 'X': break;
				default : printf("incompatible calendarEvent type\n");

			}

		//if 

	}
	pthread_exit(NULL);
}


/*****************************************************************/

int main (int argc, char* argv[]) {

	
	if (argc != 2) {
		perror("ERROR: ./program <size of buffer>");
		exit(-1);
	}

	char *endptr;
	int bufferlength = strtol(argv[1],&endptr,10); // convert string from input to int

	struct circularBuffer *cBuf = malloc(sizeof(struct circularBuffer) + bufferlength * sizeof(struct calendarEvent_t));

	cBuf->size = bufferlength; //store size of buffer in struct so can be passed to threads 

	printf("Buffer is of size %d\n", bufferlength );
	


	/*make sure to initialize buffer*/
	
	/* declare threads, init mutex, and init condition */
	pthread_t prodThread[PRODTHREADS],consThread[CONSTHREADS]; 
	pthread_mutex_init(&mtx, NULL); 
	pthread_cond_init(&prodCond, NULL);
	pthread_cond_init(&consCond, NULL);

	for (int i = 0; i < PRODTHREADS; i++){
		printf("Creating producer[%d] thread\n",i);
		if ( pthread_create( &prodThread[i],NULL,producer,(void*)cBuf ) ) { 
			perror("error creating producer thread");
			exit(-1);
		}
	}
	for (int i = 0; i < CONSTHREADS; i++){
		printf("Creating consumer[%d] thread\n",i);
		if ( pthread_create( &consThread[i],NULL,consumer,(void*)cBuf ) ) { 
			perror("error creating consumer thread");
			exit(-1);
		}
	}

	//pthread_cond_wait(&consCond,&mtx);

	/* wait for all threads to terminate, destroy mutex and condition */
	pthread_exit(NULL); 
	free(cBuf);
	pthread_mutex_destroy(&mtx);
	pthread_cond_destroy(&prodCond);
	pthread_cond_destroy(&consCond);
}

/***************************************************************************/

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