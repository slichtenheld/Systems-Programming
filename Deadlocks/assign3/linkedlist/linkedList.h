#ifndef list_H
#define list_H

typedef struct List_t *List_T;

typedef int (*listIterator)(void *);
typedef int (*compare)(void *); //this right?

extern List_T List_new(int elementSize); //only creates head node
extern void List_del(List_T list);


extern void List_prepend(List_T list, void* element);
extern void List_append(List_T list, void* element);

extern void List_remove(List_T list, )

extern void List_find(List_T list, (*compare)(void* key), void* key);




#endif