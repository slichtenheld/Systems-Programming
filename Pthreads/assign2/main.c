
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

#define COUNTMAX 20
#define PRODTHREADS 1
#define CONSTHREADS 1

/* global variables declared here*/
pthread_mutex_t mtx; // declare global mutex
pthread_cond_t prodCond,consCond ; //declare global conditions
pthread_cond_t termCond; // termination condition


int main (int argc, char* argv[]) {

	
	if (argc != 2) {
		perror("ERROR: ./program <size of buffer>");
		exit(-1);
	}

	char *endptr;
	int bufferlength = strtol(argv[1],&endptr,10); // convert string from input to int

	/*initialize buffer*/
	struct circularBuffer *cBuf = malloc(sizeof(struct circularBuffer) + bufferlength * sizeof(struct calendarEvent_t));

	cBuf->size = bufferlength; //store size of buffer in struct so can be passed to threads 

	DEBUG_PRINT( ("Buffer is of size %d\n", bufferlength ) );
	
	
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

	/* wait for all threads to terminate, destroy mutex and condition */
	pthread_exit(NULL); 
	free(cBuf);
	pthread_mutex_destroy(&mtx);
	pthread_cond_destroy(&prodCond);
	pthread_cond_destroy(&consCond);
}