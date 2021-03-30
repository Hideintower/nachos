#include "dllist.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void
Insert(int thread_num, int insert_num, DLList *list)
{
	int i, key_num;
	srand(time(NULL));
	for (i=0; i<insert_num; i++) {
		key_num = rand() % 100 + 1;
		list -> SortedInsert(NULL, key_num);
		printf("Thread %d: Insert key value %d successfully\n", thread_num, key_num);
	}
}

void
TRemove(int keyptr, int thread_num, DLList *list)
{
	list -> Remove(&keyptr);
	printf("Thread %d: Remove key value %d successfully\n", thread_num, keyptr);
}

void
TestPrepend(void *item, DLList* list)
{
	list->Prepend(item);
	list->ShowDLList();
}

void
TestAppend(void* item, DLList* list)
{
	list->Append(item);
	list->ShowDLList();
}

void
TestSortedRemove(int sortKey, DLList* list)
{
	list->SortedRemove(sortKey);
	list->ShowDLList();
}
