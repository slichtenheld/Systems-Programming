/* interface for calendar storage abstract data type */


#ifndef CALENDAR_ADT_H_
#define CALENDAR_ADT_H_
#include <stdio.h>

struct CalendarItem_t { // this needs to be declared in .h file, can't be in .c file
	char title[11];
	char date[11];
	char time[6];
	char location[11];
};


typedef struct CalendarItem_t *CalendarItem_T;
typedef struct Calendar_t *Calendar_T;

extern Calendar_T Calendar_new(void);
extern int Calendar_empty(Calendar_T cal); // returns 1 if true, 0 if false
extern int Calendar_full(Calendar_T cal); // returns 1 if true, 0 if false
extern void Calendar_add(Calendar_T cal, CalendarItem_T item);
extern void Calendar_del(Calendar_T cal, CalendarItem_T item);
extern void Calendar_mod(Calendar_T cal, CalendarItem_T item);
extern void Calendar_print(Calendar_T cal);

extern void print_calItem(struct CalendarItem_t c);
extern int cmp_calItem(struct CalendarItem_t c, struct CalendarItem_t d);

#endif