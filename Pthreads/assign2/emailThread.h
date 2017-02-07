#ifndef EMAILTHEAD_H
#define EMAILTHEAD_H

#include "calendarADT.h"

/* adds char to CalendarItem Struct */
struct calendarEvent_t {
	char type;
	struct CalendarItem_t calItem;
};

/* prints out everything in struct */
void printCalEvDebug(struct calendarEvent_t c);

/* returns -1 if error parsing, ie not proper format */
/* takes in line in buffer and populates struct accordingly */ 
int parseEmail(char * buffer, struct calendarEvent_t *c);

// /* reads in stdin, parses, places in buffer passed as struct*/
// /* arg is of type (struct circularBuffer*) */
//void * producer(void * arg);

#endif