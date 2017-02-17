#include "list.h"
#include "iterator.h"
#include <stdlib.h> //free
#include <string.h> //memcpy
#include <assert.h> //assert remove
#include <stdio.h> //TODO remove

//TODO: delete certain nodes
//TODO: iterator

struct node_t {
	void * data;
	struct node_t * next;
};

struct List_t {
	int elementSize;
	struct node_t* head; // needs to be changed on prepend
	struct node_t* tail; // needs to be changed on append
};

struct Iter_t {
	struct node_t * head; // only pointers, don't need to be malloc'd
	struct node_t *current;
};

List_T List_new(int elementSize) {
	List_T list = malloc(sizeof(struct List_t));
	list->elementSize = elementSize;
	list->head = NULL;
	list->tail = NULL;
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

//UNTESTED
// void* List_search(List_T list, void* key,searchFunction searchFn){
// 	struct node_t *current; //need pointer to iterate through data structure
	
// 	current = list->head;
// 	while(current!= NULL){
// 		if (searchFn(current->data, key))
// 			return current->data;
// 		current = current->next;
// 	}
// }

void List_printAll(List_T list, printFunction printFn){
	struct node_t *current; //need pointer to iterate through data structure
	
	current = list->head;
	while(current!= NULL){
		printFn(current->data);		/* print data */
		current = current->next;
	}
}

Iter_T List_makeIter(List_T list){

	Iter_T iter = malloc(sizeof(struct Iter_t));
	assert(list->head != NULL);
	iter->head = list->head;
	iter->current = iter->head;

	return iter;
}

int Iter_isValid(Iter_T iter){
	if (iter->current==NULL)
		return 0;
	else 
		return 1;
}

void Iter_next(Iter_T iter){
	iter->current = iter->current->next;
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