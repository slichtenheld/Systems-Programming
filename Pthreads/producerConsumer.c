/* example of solving the producer consumer problems */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BufferSize 5

/* global variables, is this the best way? */
pthread_mutex_t mtx; // declare global mutex
int counter = 0;
#define COUNTMAX 20

struct circularBuffer{
	int in;
	int out;
	int numItems;
	int buffer[BufferSize];
}; 

void * producer(void * arg) {
	struct circularBuffer *circbuf = (struct circularBuffer*) arg;
	for (; counter < COUNTMAX; counter++){
		if ( circbuf->numItems < BufferSize ) {
			circbuf->buffer[circbuf->in] = counter;
			circbuf->in  = (circbuf->in + 1) % BufferSize;
			circbuf->numItems++;
			printf("adding %d to buffer, in: %d, numItems: %d\n", counter,circbuf->in,circbuf->numItems);
		}
		else counter--;
	}
}

void * consumer(void * arg) {
	struct circularBuffer *circbuf = (struct circularBuffer*) arg;
	while(counter < COUNTMAX){
		if (circbuf->numItems > 0){
			printf("Removing %d from buffer\n",circbuf->buffer[circbuf->out] );
			circbuf->out = (circbuf->out + 1) % BufferSize;
			circbuf->numItems --;
		}
	}
}


/*****************************************************************/

int main (int argc, char* argv[]) {

	/*
	if (argc != 2) {
		perror("ERROR: ./program <size of buffer>");
		exit(-1);
	}

	char *endptr;
	int bufferlength = strtol(argv[1],&endptr,10);

	printf("Buffer is of size %d\n", bufferlength );
	*/
	struct circularBuffer cBuf = {0,0,0,{0,0,0,0,0}}; /*make sure to initialize buffer*/
	pthread_t prodThread,consThread; /* declare threads */
	pthread_mutex_init(&mtx, NULL); /*create mutex*/

	printf("Creating producer thread\n");
	if ( pthread_create( &prodThread,NULL,producer,(void*)&cBuf ) ) { 
		perror("error creating producer thread");
		exit(-1);
	}

	printf("Creating consumer thread\n");

	if ( pthread_create( &consThread,NULL,consumer,(void*)&cBuf ) ) { 
		perror("error creating consumer thread");
		exit(-1);
	}


	pthread_exit(NULL); /* wait for all threads to terminate */
	pthread_mutex_destroy(&mtx);
}