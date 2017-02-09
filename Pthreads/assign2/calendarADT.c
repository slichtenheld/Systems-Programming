#include "calendarADT.h"
#include <stdio.h>
#include <stdlib.h> // for malloc
#include <string.h> // for string compare

#define CAPACITY 1024



struct Calendar_t {
	int count;
	CalendarItem_T data[CAPACITY];
};

Calendar_T Calendar_new(void){
	
	Calendar_T cal; 
	
	//printf("Size of Calendar: %lu\n", sizeof *cal);
	cal = malloc(sizeof *cal);

	if (cal==NULL) perror("NEW, calendar is null");
	cal->count = 0; //array is empty
	return cal;
}

int Calendar_empty(Calendar_T cal){
	if (cal==NULL) perror("EMPTY, calendar is null");
	return (cal->count == 0);
}

int Calendar_full(Calendar_T cal){
	if (cal==NULL) perror("FULL, calendar is null");
	return (cal->count == CAPACITY-1);
}

int time2int(char* charString){ // needs formate of HH:MM
	char numArray[]={'0','1','2','3','4','5','6','7','8','9'};
	int temp = 0;
	for (int i = 0; i < strlen(charString) ; i++){
		
		for (int j = 0; j < 10; j++){

			if (charString[i]==numArray[j]){
				switch(i){
					case(0): temp += 10*j*60; break;
					case(1): temp += j*60; break;
					//case(2): /* do nothing, ignore because of semicolon */ break;
					case(3): temp += j*10; break;
					case(4): temp += j*10; break;
					default: ;
				}
			}			
		}
	}
	return temp;
}

/* returns true if earliest item on that day*/
int cmp_earliestItem(Calendar_T cal, CalendarItem_T item){ // only compares date
	uint boolean = 0;
	int lowestTime = time2int(item->time); // initialized to highest value
	int matchTime = 0; 
	for(int i = 0; i < cal->count; i++){
		/*check if any of the dates exist already and if earlier time */
		if ( 
			( strcmp( (*(cal->data[i])).date, item->date) == 0 ) && 
			( time2int( (cal->data[i])->time) < lowestTime ) 
			)
		{ 
			lowestTime = time2int(cal->data[i]->time);
		}
	} 
	if (time2int(item->time) <= lowestTime) return 1; 
	else return 0;
}


void Calendar_add(Calendar_T cal, CalendarItem_T item){

	if (cal==NULL) {
		perror("ADD, calendar is null");
		return;
	}
	if ( cmp_earliestItem(cal,item) ) { 	
		printf("%s:%s\n",item->date,item->location);
	}
	if (cal->count < CAPACITY){
		cal->data[cal->count++] = item;
	}
	else perror("calendar is full");
}

void Calendar_del(Calendar_T cal, CalendarItem_T item){
	if (cal==NULL) {
		perror("DEL, calendar is null");
		return;
	}

	// check if anythings are equal, if it is shift everything up
	for(int i = 0; i < cal->count; i++){
		if ( // if title, date, and time are equal => delete
			strcmp((*(cal->data[i])).title,item->title) == 0 &&
			strcmp((*(cal->data[i])).date,item->date) == 0 &&
			strcmp((*(cal->data[i])).time,item->time) == 0) 
		{
			for (;i < cal->count -1; i++){ // since implemented as array, order can make a difference since comparing only name
				cal->data[i] = cal->data[i + 1];
			}
			cal->count = cal->count - 1;
			break;
		}
	}

	if ( cmp_earliestItem(cal,item) ) { 	
		printf("%s:None\n",item->date);
	}
}

void Calendar_mod(Calendar_T cal, CalendarItem_T item){
	if (cal==NULL) perror("MOD, calendar is null");

	if ( cmp_earliestItem(cal,item) ) { 	
		printf("%s:%s\n",item->date,item->location);
	}

	for(int i = 0; i < cal->count; i++){
		if ( strcmp((*(cal->data[i])).title,item->title) == 0 )
			memcpy(cal->data[i],item, sizeof(struct CalendarItem_t));
	}
}

void Calendar_print(Calendar_T cal){
	if (cal==NULL) perror("MOD, calendar is null");
	
	for (int i = 0; i < cal->count; i++){
		print_calItem(*(cal->data[i]));
	}
}

/****private methods****/
void print_calItem(struct CalendarItem_t c){
	printf("Title: %s, Date: %s, Time: %s, location: %s\n\n", c.title, c.date, c.time, c.location);
}



// void cmp_delItem(Calendar_T cal, CalendarItem_T item){
// 	uint boolean = 0;
// 	for(int i = 0; i < cal->count; i++){
// 		if ( 	
// 				(strcmp((*(cal->data[i])).date,item->date)) == 0 &&
// 				(strcmp((*(cal->data[i])).title,item->title) == 0)
// 			) 
// 			{ /* check if any of the dates exist already */
// 			boolean = 1;
// 			break;
// 		}
// 	}
// 	if (boolean==0) printf("%s:None\n",item->date);
// }



// mod function old algorithm from assign1
	// int lowestTime = 1440;
	// int matchTime = 0;
	// for(int i = 0; i < cal->count; i++){
	// 	if (strcmp((*(cal->data[i])).title,item->title) == 0) { /*compares title and date */

	// 		if (time2int(item->time)<lowestTime) lowestTime = time2int(item->time);

	// 		if ( strcmp((*(cal->data[i])).time,item->time) == 0 ) {
	// 			memcpy(cal->data[i]->time,item->time, strlen(item->time)+1); // update time
	// 			memcpy(cal->data[i]->location,item->location, strlen(item->location)+1); // update location
	// 			memcpy(cal->data[i]->date,item->location, strlen(item->date)+1);
	// 			matchTime = time2int(item->time);
	// 		}
	// 	}
	// }