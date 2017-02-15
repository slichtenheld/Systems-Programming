/* Created by SAMUEL LICHTENHELD */
 
/*********************************
 NOTES
	- 2/14/17 added semaphores to make pushing and popping threadsafe
	- OTHER FUNCTIONS ARE **NOT** THREAD SAFE
		- shouldn't need other functions however, since popping will 
		  block if nothing there
	- WILL BLOCK IF TRYING TO POP WHEN EMPTY
	- WILL BLOCK IF TRYING TO PUSH WHEN FULL
	- MUST USE HEAP WHEN USING MULTIPLE THREADS
	- essentially, right now will only work properly with multiple threads,
	  but don't know when one would be using FIFOs in single threaded applications
**********************************/

/*********************************
IMPLEMENTATION DETAILS
 	- pushing and popping when full and empty respectively
 	 will not affect the underlying buffer. 
	- Pushing when full will return -1. 
	- Popping when full return an old pointer. 
		- This decision was made as garbage data 
		  is better than segfaults.
**********************************/


#include "fifo.h"
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>

struct Fifo_t {
	int size;
	int in;
	int out;
	int numItems;
	sem_t m,f,e; // semaphores for mutual exclusion, full, empty respectively
	void * buffer[];
};


Fifo_T Fifo_new(int size) {
	/* allocate memory */
	Fifo_T fifo = malloc(sizeof(struct Fifo_t)+size*sizeof(void *));
	assert (fifo != NULL);
	/* store_size */
	fifo->size=size;
	fifo->in=0;
	fifo->out=0;
	fifo->numItems=0;
	/* instantiate semaphores */
	sem_init(&(fifo->m),0,1); //mutual exclusion, must be 1 so that producer can acquire buffer first
	sem_init(&(fifo->f),0,size); // full tokens 
	sem_init(&(fifo->e),0,0); //empty tokens 
	return fifo;
}
void Fifo_del(Fifo_T fifo){
	assert(fifo);
	/* free semaphores */
	sem_destroy(&(fifo->m)); 
	sem_destroy(&(fifo->f));
	sem_destroy(&(fifo->e));
	/* free fifo */
	free(fifo);
}
int Fifo_push(Fifo_T fifo, void *item){
	assert(fifo); 

	// down f // order matters, if down m first, deadlock
	sem_wait(&(fifo->f));
	// down m
	sem_wait(&(fifo->m));
	
	// check if FULL
	// if(fifo->numItems==fifo->size)
	// 	return -1;

	fifo->buffer[fifo->in] = item;
	fifo->in = (fifo->in + 1) % (fifo->size);
	fifo->numItems++;

	// up(e) // order doesn't matter, ups don't block	 	
	sem_post(&(fifo->e));
	// up(m) 
	sem_post(&(fifo->m));

	return 1;
}
void* Fifo_pop(Fifo_T fifo){
	assert(fifo);

	// down(e)
	sem_wait(&(fifo->e));
	// down(m)
	sem_wait(&(fifo->m));

	/* check if empty */
	// if (fifo->numItems==0)
	// 	return fifo->buffer[0]; // garbage pointer

	void * temp = fifo->buffer[fifo->out];
	fifo->out = (fifo->out + 1)% (fifo->size);
	fifo->numItems--;

	// up(f)
	sem_post(&(fifo->f));
	// up (m)
	sem_post(&(fifo->m));
	return temp;
}
int Fifo_size(Fifo_T fifo){
	assert(fifo);

	return fifo->size;
}

int Fifo_isEmpty(Fifo_T fifo){
	return (fifo->numItems==0);
}

int Fifo_notEmpty(Fifo_T fifo){
	return !(fifo->numItems==0);
}

int Fifo_isFull(Fifo_T fifo){
	return (fifo->numItems==fifo->size);
}

int Fifo_numItems(Fifo_T fifo){
	assert(fifo);

	return fifo->numItems;
}