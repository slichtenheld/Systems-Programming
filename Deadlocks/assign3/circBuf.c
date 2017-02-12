#include "fifo.h"
#include <stdlib.h>
#include <assert.h>


struct Fifo_t {
	int size; // TODO: necessary?
	int in;
	int out;
	int numItems;
	void * buffer[];
};


Fifo_T Fifo_new(int size) {
	/* allocate memory */
	Fifo_T fifo_rtn = malloc(sizeof(struct Fifo_t)+size*sizeof(void *));
	assert (fifo_rtn != NULL);
	/* store_size */
	fifo_rtn->size=size; // TODO: necessary?
	fifo_rtn->in=0;
	fifo_rtn->out=0;
	fifo_rtn->numItems=0;
	return fifo_rtn;
}
void Fifo_del(Fifo_T fifo){
	assert(fifo);

	free(fifo);
}
int Fifo_push(Fifo_T fifo, void *item){
	assert(fifo);

	//check if FULL
	if(fifo->numItems==fifo->size)
		return -1;

	fifo->buffer[fifo->in] = item;
	fifo->in = (fifo->in + 1) % (fifo->size);
	fifo->numItems++;

	return 1;
}
void* Fifo_pop(Fifo_T fifo){
	assert(fifo);

	/* check if empty */
	if (fifo->numItems==0)
		return fifo->buffer[0]; // garbage pointer

	void * temp = fifo->buffer[fifo->out];
	fifo->out = (fifo->out + 1)% (fifo->size);
	fifo->numItems--;
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