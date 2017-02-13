#ifndef FIFO_H
#define FIFO_H

/* FIFO */
/* Notes: pushing and popping when 
full and empty respectively will not
affect the underlying buffer. 
**Pushing when full will return -1. 
**Popping when full return an old pointer. 
This decision was made as garbage data 
is better than segfaults. */

typedef struct Fifo_t *Fifo_T;

extern Fifo_T Fifo_new(int size);
extern void Fifo_del(Fifo_T fifo);

extern int Fifo_push(Fifo_T fifo, void *item); //returns -1 if full
extern void* Fifo_pop(Fifo_T fifo);

extern int Fifo_isEmpty(Fifo_T fifo);
extern int Fifo_notEmpty(Fifo_T fifo);
extern int Fifo_isFull(Fifo_T fifo);

extern int Fifo_size(Fifo_T fifo);
extern int Fifo_numItems(Fifo_T fifo);

#endif
