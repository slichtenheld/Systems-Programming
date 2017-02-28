#include <stdio.h>
#include <stdlib.h> //for sizeof


#include "list.h"
#include "iterator.h"

struct testStruct {
	int num;
	char letter;
};

void printtestStruct(void *temp) {
	struct testStruct * ts = (struct testStruct*)temp;
	
	printf("TEST: %d%c\n",ts->num,ts->letter );
}


int main( int argc, char * argv[] ) {
	

	List_T list = List_new(sizeof(struct testStruct));

	struct testStruct ts = { 1,'A' };
	List_prepend(list,&ts);



	//struct testStruct ts2 = { 2,'B' };
	ts.num = 3;
	ts.letter = 'C';

	List_prepend(list,&ts);



	ts.num = 4;
	ts.letter = 'D';
	List_append(list,&ts);



	ts.num = 5;
	ts.letter = 'E';
	List_append(list,&ts);

	ts.num = 6;
	ts.letter = 'F';
	List_prepend(list,&ts);



	List_printAll(list,printtestStruct);

	printf("********************\n");

	Iter_T iter = List_makeIter(list);
	for(Iter_rst(iter); Iter_isValid(iter); Iter_next(iter)){
		struct testStruct* temp;
		temp = Iter_currentItem(iter);
		printf("%d%c\n",temp->num,temp->letter );
	}
	printf("- - - -- -- - - -\n");
	for(Iter_rst(iter); Iter_isValid(iter); Iter_next(iter)){
		struct testStruct* temp;
		temp = Iter_currentItem(iter);
		if(temp->num == 4)
			temp->letter = 'Z';
	}

	printf("______________________\n");

	List_printAll(list,printtestStruct);


	List_free(list);
}