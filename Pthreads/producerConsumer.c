/* example of solving the producer consumer problems */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BufferSize 5

/* global variables, is this the best way? */
pthread_mutex_t mtx; // declare global mutex
pthread_cond_t prodCond,consCond ; //declare global conditions

int counter = 0;
#define COUNTMAX 20
#define PRODTHREADS 2
#define CONSTHREADS 2

struct circularBuffer{
	int in;
	int out;
	int numItems;
	int buffer[BufferSize];
}; 

void * producer(void * arg) {
	struct circularBuffer *circbuf = (struct circularBuffer*) arg;
	while(1) {

		pthread_mutex_lock(&mtx); // acquire lock

		/* give up lock if buffer is full and wait to be woken up */
		while ( circbuf->numItems >= BufferSize ) // while loop because thread could be woken up due to other conditions
			pthread_cond_wait(&prodCond,&mtx);

		printf("adding %d to buffer, in: %d, numItems: %d\n", counter,circbuf->in,circbuf->numItems);
		circbuf->buffer[circbuf->in] = counter++;
		circbuf->in  = (circbuf->in + 1) % BufferSize;
		circbuf->numItems++;

		if (circbuf->numItems >= 1) 
			pthread_cond_signal(&consCond);

		if (counter > COUNTMAX) {
			pthread_mutex_unlock(&mtx);
			pthread_exit(NULL);
		}

		pthread_mutex_unlock(&mtx);

	}
}

void * consumer(void * arg) {
	struct circularBuffer *circbuf = (struct circularBuffer*) arg;
	while(1) { 

		pthread_mutex_lock(&mtx); // acquire lock

		/* give up lock if buffer is full and wait to be woken up */
		while ( circbuf->numItems <= 0 ) // while loop because thread could be woken up due to other conditions
			pthread_cond_wait(&consCond,&mtx);

		printf("Removing %d from buffer\n",circbuf->buffer[circbuf->out] );
		circbuf->out = (circbuf->out + 1) % BufferSize;
		circbuf->numItems --;

		if (circbuf->numItems <= BufferSize) 
			pthread_cond_signal(&prodCond);

		if (counter > COUNTMAX) {
			pthread_mutex_unlock(&mtx);
			pthread_exit(NULL);
		}

		pthread_mutex_unlock(&mtx);
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


	/*make sure to initialize buffer*/
	struct circularBuffer cBuf = {0,0,0,{0,0,0,0,0}}; 
	/* declare threads, init mutex, and init condition */
	pthread_t prodThread[PRODTHREADS],consThread[CONSTHREADS]; 
	pthread_mutex_init(&mtx, NULL); 
	pthread_cond_init(&prodCond, NULL);
	pthread_cond_init(&consCond, NULL);

	for (int i = 0; i < PRODTHREADS; i++){
		printf("Creating producer[%d] thread\n",i);
		if ( pthread_create( &prodThread[i],NULL,producer,(void*)&cBuf ) ) { 
			perror("error creating producer thread");
			exit(-1);
		}
	}
	for (int i = 0; i < CONSTHREADS; i++){
		printf("Creating consumer[%d] thread\n",i);
		if ( pthread_create( &consThread[i],NULL,consumer,(void*)&cBuf ) ) { 
			perror("error creating consumer thread");
			exit(-1);
		}
	}
	/* wait for all threads to terminate, destroy mutex and condition */
	pthread_exit(NULL); 
	pthread_mutex_destroy(&mtx);
	pthread_cond_destroy(&prodCond);
	pthread_cond_destroy(&consCond);
}