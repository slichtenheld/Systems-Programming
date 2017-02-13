#ifndef ITERATOR_H
#define ITERATOR_H

typedef struct Iter_t* Iter_T;

extern Iter_T Iter_new();
extern void Iter_free(Iter_T iter);

extern int Iter_isValid(Iter_T iter);
extern int Iter_next(Iter_T iter);
extern void* Iter_currentItem(Iter_T iter);

extern void Iter_rst(Iter_T iter);

#endif