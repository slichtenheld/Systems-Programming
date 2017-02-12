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
void Fifo_push(Fifo_T fifo, void *item){
	assert(fifo);

	fifo->buffer[fifo->in] = item;
	fifo->in = (fifo->in + 1) % (fifo->size);
	fifo->numItems++;
}
void* Fifo_pop(Fifo_T fifo){
	assert(fifo);

	void * temp = fifo->buffer[fifo->out];
	fifo->out = (fifo->out + 1)% (fifo->size);
	fifo->numItems--;
	return temp;
}
extern int Fifo_size(Fifo_T fifo){
	assert(fifo);

	return fifo->size;
}
extern int Fifo_numItems(Fifo_T fifo){
	assert(fifo);

	return fifo->numItems;
}