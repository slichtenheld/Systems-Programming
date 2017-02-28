#ifndef list_H
#define list_H

#include "iterator.h"

typedef void (*printFunction)(void *);
typedef int (*searchFunction)(void *, void *);

typedef struct List_t *List_T;

//typedef int (*listIterator)(void *);
//typedef int (*compare)(void *); //this right?

extern List_T List_new(int elementSize); //only creates head and tail node pointers, set to NULL
extern void List_free(List_T list); //frees all allocated memory on list


extern void List_prepend(List_T list, void* element); // adds element to beginning of list
extern void List_append(List_T list, void* element); // adds element to end of list

extern void* List_search(List_T list, void* key,searchFunction searchFn); //uses key to search and returns data associated with it

extern Iter_T List_makeIter(List_T list);

extern void List_printAll(List_T list, printFunction printFn);

//extern void List_remove(List_T list, );

//extern void List_find(List_T list, (*compare)(void* key), void* key);




#endif