#include <fifo.h>



struct Fifo_t {
	int size;
	int in;
	int out;
	int numItems;
	void * items[];
}


Fifo_T Fifo_new(int size) {
	Fifo_T fifo_rtn = malloc(sizeof(struct Fifo_t)+size*sizeof(void *));
	fifo_rtn->size=size;
}
void Fifo_del(Fifo_T fifo){

}
void Fifo_push(Fifo_T fifo, void *item){

}
void* Fifo_pop(Fifo_T fifo){

}