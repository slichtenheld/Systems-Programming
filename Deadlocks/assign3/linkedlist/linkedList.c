#include "linkedList.h"
#include <stdlib.h> //free


TODO: fix append prepend logic

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

List_T List_new(int elementSize) {
	struct List_T list = malloc(sizeof(List_t));
	list->size = 0;
	list->elementSize=0; // TODO: what this used for?
	list->elementSize = elementSize;
	return list;
}

void List_del(List_T list){
	//free each nodes data
	//free each node
	//then free List
}

void List_prepend(List_T list, void* element){
	node_t *node = malloc(sizeof(node_t));
	node->data = malloc(list->elementSize);
	memcpy(node->data,element,list->elementSize);

	node->next = list->head;
	list->head = node;

	// first node?
	if(list->tail==NULL)
		list->tail = list-> head;

}

void List_append(List_T list, void* element){
	node_t *node = malloc(sizeof(node_t));
	node->data = malloc(list->elementSize);
	memcpy(node->data,element,list->elementSize);

	node->next = NULL;

	if(list->head == NULL){
		list->head = node;
		
	}
	list->tail = node;
}


void push() 