#ifndef FIFO_H
#define FIFO_H


typedef struct Fifo *Fifo_T;

extern Fifo_T Fifo_new(int size);
extern void Fifo_del(Fifo_T fifo);

extern void Fifo_push(Fifo_T fifo, void *item);
extern void* Fifo_pop(Fifo_T fifo);


#endif
