#ifndef CIRCBUF_H
#define CIRCBUF_H

#include "emailThread.h"

struct circularBuffer{
	int size;
	int in;
	int out;
	int numItems;
	struct calendarEvent_t buffer[];
};

#endif 