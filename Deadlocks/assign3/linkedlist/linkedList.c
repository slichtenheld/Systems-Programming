#include "linkedList.h"
#include <stdlib.h> //free
#include <string.h> //memcpy


//TODO: fix append prepend logic

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


