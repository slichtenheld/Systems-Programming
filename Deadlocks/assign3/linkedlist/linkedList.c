#include "linkedList.h"
#include "iterator.h"
#include <stdlib.h> //free
#include <string.h> //memcpy


//TODO: delete certain nodes
//TODO: iterator

struct node_t {
	void * data;
	struct node_t * next;
};

struct List_t {
	int size; // TODO: necessary?
	int elementSize;
	struct node_t* head;
	struct node_t* tail;
};

struct Iter_t {
	struct node_t * head; // only pointers, don't need to be malloc'd
	struct node_t *current;
}

List_T List_new(int elementSize) {
	List_T list = malloc(sizeof(struct List_t));
	list->size = 0;
	list->elementSize=0; // TODO: what this used for?
	list->elementSize = elementSize;
	return list;
}

void List_free(List_T list){
	struct node_t *current; //need pointer to iterate through data structure
	while(list->head != NULL){
		/* isolate node from head of list*/
		current = list->head;
		list->head = current->next;

		/* frees data from node and node itself */
		free(current->data); // TODO: will this free everything?
		free(current);
	}
}

void List_prepend(List_T list, void* element){
	struct node_t *node = malloc(sizeof(struct node_t));
	node->data = malloc(list->elementSize);
	memcpy(node->data,element,list->elementSize);

	node->next = NULL;

	/* if list empty, ie first node */
	if(list->head==NULL){
		list->head = node;
		list->tail = node;
	}
	else{
		node->next = list->head;
		list->head = node;
	}
}

void List_append(List_T list, void* element){
	struct node_t *node = malloc(sizeof(struct node_t));
	node->data = malloc(list->elementSize);
	memcpy(node->data,element,list->elementSize);

	node->next = NULL;

	/* if list empty, ie first node */
	if(list->head == NULL){
		list->head = node;
		list->tail = node;
	}
	else{
		list->tail->next = node;
		list->tail = node;
	}
	
}

Iter_T List_makeIter(List_T list){

	Iter_T iter = malloc(sizeof(struct Iter_t));
	iter->head = list->head;
	iter->current = iter->head;

	return iter;
}

// struct Iter_t {
// 	struct node_t * next;
// }

int Iter_isValid(Iter_T iter){
	if (iter->current->next==NULL)
		return 0;
	else 
		return 1;
}
int Iter_next(Iter_T iter){
	if (iter->current->next==NULL)
		return -1;

	iter->current = iter->current->next;
	return 1;
}
void* Iter_currentItem(Iter_T iter){
	return iter->current->data;
}

int Iter_rst(Iter_T iter){
	if (iter->head != NULL) {
		iter->current = iter->head;
		return 1;
	}
	else
		return -1;
}