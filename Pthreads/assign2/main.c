
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "calendarADT.h"
#include "macros.h"
#include "emailThread.h"
#include "calThread.h"
#include "circBuf.h"
#include "mtxGlbl.h"

#define gettid() syscall(SYS_gettid)

#define COUNTMAX 20
#define PRODTHREADS 1
#define CONSTHREADS 1

/* global variables declared here*/
pthread_mutex_t mtx; // declare global mutex
pthread_cond_t prodCond,consCond ; //declare global conditions
pthread_cond_t termCond; // termination condition


// void * consumer(void * arg) {
// 	struct circularBuffer *circbuf = (struct circularBuffer*) arg;

// 	Calendar_T cal = Calendar_new();
// 	char type;
		

// 	/* loop until poison pill received */
// 	while(1) { 
// 		DEBUG_PRINT( ("THREAD C: %ld\n", gettid() ) );

// 		struct CalendarItem_t *item = malloc(sizeof *item);

// 		/* CRITICAL SECTION */
// 		pthread_mutex_lock(&mtx); // acquire lock
// 		{
// 			/* give up lock if buffer is full and wait to be woken up */
// 			while ( circbuf->numItems == 0 ) // while loop because thread could be woken up due to other conditions
// 				pthread_cond_wait(&consCond,&mtx);

// 			/* copy buffer item to local item */
// 			memcpy( item, &(circbuf->buffer[circbuf->out].calItem), sizeof(struct CalendarItem_t) ); // copy struct to buffer
// 			type = circbuf->buffer[circbuf->out].type;
// 			circbuf->out = (circbuf->out + 1) % circbuf->size;
// 			circbuf->numItems --;

// 			/*** OPTIMIZATION: PRODUCER WILL ONlY BE SLEEPING IF BUFFER WAS FULL, AND NUMITEMS JUST GOT DECREMENTED BY 1 ***/ 
// 			if (circbuf->numItems == circbuf->size - 1) 
// 				pthread_cond_signal(&prodCond);
// 		}
// 		pthread_mutex_unlock(&mtx);
// 		/* END CRITICAL SECTION */
		
// 		switch (type){
// 			case 'E': pthread_exit(NULL); break;
// 			case 'C': Calendar_add(cal, item);break;
// 			case 'D': Calendar_del(cal, item);break;
// 			case 'X': Calendar_mod(cal, item);break;
// 			default : printf("incompatible calendarEvent type\n");
// 		}
// 	}
// }


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

	DEBUG_PRINT( ("Buffer is of size %d\n", bufferlength ) );
	


	/*make sure to initialize buffer*/
	
	/* declare threads, init mutex, and init condition */
	pthread_t prodThread[PRODTHREADS],consThread[CONSTHREADS]; 
	pthread_mutex_init(&mtx, NULL); 
	pthread_cond_init(&prodCond, NULL);
	pthread_cond_init(&consCond, NULL);

	for (int i = 0; i < PRODTHREADS; i++){
		DEBUG_PRINT( ("Creating producer[%d] thread\n",i) );
		if ( pthread_create( &prodThread[i],NULL,producer,(void*)cBuf ) ) { 
			perror("error creating producer thread");
			exit(-1);
		}
	}
	for (int i = 0; i < CONSTHREADS; i++){
		DEBUG_PRINT( ("Creating consumer[%d] thread\n",i) );
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