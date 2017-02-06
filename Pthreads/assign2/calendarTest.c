#include <stdio.h>
#include "calendarADT.h"


int main(int argc, char *argv[]) {
	
	Calendar_T cal = Calendar_new();
	printf("empty: %s\n", Calendar_empty(cal) ? "true" : "false");

	struct CalendarItem_t calItem; // = {"Meeting   ","01/12/2017","15:30","NEB202    "};
	//struct CalendarItem_t calItem2 = {"Meeting2  ","01/12/2017","17:00","LAR313    "};
	//struct CalendarItem_t calItem3 = {"Meeting3  ","01/15/2017","21:00","BLABLA    "};

	calItem = {"Meeting   ","01/12/2017","15:30","NEB202  "};
	Calendar_add(cal, &calItem);
	calItem = {"Meeting   ","01/19/2017","15:30","NEB202  "};
	Calendar_add(cal, &calItem);
	calItem = {"Class     ","01/13/2017","09:00","LAR239  "};
	Calendar_add(cal, &calItem);
	calItem = {"Meeting   ","01/13/2017","12:30","MAEB300 "};
	Calendar_add(cal, &calItem);
	calItem = {"Info      ","01/13/2017","11:30","BEN216  "};
	Calendar_add(cal, &calItem);
	calItem = {"Info      ","01/13/2017","11:30","BEN216  "};
	Calendar_add(cal, &calItem);
	//Calendar_add(cal, &calItem2);
	//Calendar_add(cal, &calItem3);
	//printf("empty: %s\n", Calendar_empty(cal) ? "true" : "false");
	printf("_____________________________________\n\n");
	//Calendar_print(cal);

	//printf("equal: %s\n", cmp_calItem(calItem,calItem2) ? "true" : "false");

	printf("_____________________________________\n\n");

	Calendar_del(cal, &calItem);
	Calendar_print(cal);


	}

	/*

	C,Meeting   ,01/12/2017,15:30,NEB202    
C,Meeting   ,01/19/2017,15:30,NEB202    
C,Class     ,01/13/2017,09:00,LAR239    
C,Meeting   ,01/13/2017,12:30,MAEB300   
C,Info      ,01/13/2017,11:30,BEN216    
D,Meeting   ,01/19/2017,15:30,NEB202    
X,Class     ,01/13/2017,09:00,LAR225    
X,Info      ,01/13/2017,13:30,LAR216 

*/