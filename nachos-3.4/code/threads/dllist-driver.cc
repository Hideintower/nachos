#include "dllist.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "synch.h"


Lock mylock("dllist Lock");
Condition mycondition("dllist Condition");

void
Initialize()
{
	srand(time(NULL));
}

void
Insert(int thread_num, int insert_num, DLList *list)
{
	mylock.Acquire();
	int i, key_num;
	//srand(time(NULL));
	for (i=0; i<insert_num; i++) {
		key_num = rand() % 100 + 1;
		list -> SortedInsert(NULL, key_num);
		printf("Thread %d: Insert key value %d successfully\n", thread_num, key_num);
	}
	mycondition.Signal(&mylock);
	mylock.Release();
}

void
TRemove(int keyptr, int thread_num, int remove_num, DLList *list)
{
	mylock.Acquire();
	for(int i=0; i< remove_num; i++) {
		while(!list->IsEmpty())
			mycondition.Wait(&mylock);
		list -> Remove(&keyptr);
		printf("Thread %d: Remove key value %d successfully\n", thread_num, keyptr);
	}
	mylock.Release();
}

void
TestPrepend(void *item, DLList* list)
{
	mylock.Acquire();
	list->Prepend(item);
	mycondition.Signal(&mylock);
	list->ShowDLList();
	mylock.Release();
}

void
TestAppend(void* item, DLList* list)
{
	mylock.Acquire();
	list->Append(item);
	mycondition.Signal(&mylock);
	list->ShowDLList();
	mylock.Release();
}

void
TestSortedRemove(int sortKey, DLList* list)
{
	mylock.Acquire();
	while(!list->IsEmpty())
		mycondition.Wait(&mylock);
	list->SortedRemove(sortKey);
	list->ShowDLList();
	mylock.Release();
}
