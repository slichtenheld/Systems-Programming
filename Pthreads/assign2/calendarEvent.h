#ifndef CALENDER_EVENT_H_
#define CALENDER_EVENT_H_

struct calendarEvent_t {
	char type;
	char title[11];
	char date[11];
	char time[6];
	char location[11];
};

void printCalendarEvent(struct calendarEvent_t c){
	printf("%c,%s,%s,%s,%s\n",c.type, c.title, c.date, c.time, c.location);
}

void printCalEvDebug(struct calendarEvent_t c){
	printf("Type: %c, Title: %s, Date: %s, Time: %s, location: %s\n\n",c.type, c.title, c.date, c.time, c.location);
}

#endif