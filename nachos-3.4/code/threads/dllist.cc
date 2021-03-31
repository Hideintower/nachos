#include "dllist.h"
#include "copyright.h"
#include "utility.h"
#include "system.h"

int error_type = 0;
/*
错误类型：
0：无错误
1：插入时切换线程，导致删除链表删除错误，即：插入 = 删除 -> 插入 > 删除
2：在prepend中，判断链表为空后切换线程
3：在prepend中，判断链表为空，并确定头部指针后切换线程
4：在prepend中，判断链表为非空后切换线程
5：在prepend中，判断链表为非空，设置temp指针前切换线程
6：在prepend中，判断链表为非空，在设置插入key值前切换线程
7：在prepend中，判断链表为非空，在重新设置头指针前切换线程
8：在append中，判断链表为空后切换线程
9：在append中，判断链表为空，并确定头指针后切换线程
10：在append中，判断链表为非空后，切换线程
11：在append中，判断链表为非空，设置temp指针前切换线程
12：在append中，判断链表为非空，在设置插入key值前切换线程
13：在append中，判断链表为非空，在重新设置尾指针前切换线程
14：在remove中，非空链表，赋值头指针前切换线程
15：在remove中，单节点链表，头指针赋空值前切换线程
16：在remove中，单节点链表，尾指针赋空值前切换线程
17：在remove中，多节点链表，重设头指针前切换线程
18：在sortedinsert中，判断链表为空后，切换线程
19：在sortedinsert中，判断链表为空，设置尾指针前切换线程
20：
21：
22：
23：
24：
25：
26：
27：
28：
29：
*/

DLLElement::DLLElement(void *itemPtr, int sortKey)
{
	item = itemPtr;
	key = sortKey;
	next = NULL;
	prev = NULL;
}

DLList::DLList()
{
	first = NULL;
	last = NULL;
}

DLList::~DLList()
{
	while ( Remove(NULL) );
}

void
DLList::Prepend(void *item)
{
	DLLElement *temp = new DLLElement(item, 0);
	if (!IsEmpty()) {
		if (error_type == 2)
			currentThread->Yield();
		first = temp;
		if (error_type == 3)
			currentThread->Yield();
		last = temp;
		temp->key = 1;
	}
	else {
		if (error_type == 4)
			currentThread->Yield();
		first->prev = temp;
		if (error_type == 5)
			currentThread->Yield();
		temp->next = first;
		if (error_type == 6)
			currentThread->Yield();
		temp->key = first->key - 1;
		if (error_type == 7)
			currentThread->Yield();
		first = temp;
	}
}

void
DLList::Append(void *item)
{
	DLLElement *temp = new DLLElement(item, 0);
	if (!IsEmpty()) {
		if (error_type == 8)
			currentThread->Yield();
		first = temp;
		if (error_type == 9)
			currentThread->Yield();
		last = temp;
		temp->key = 1;
	}
	else {
		if (error_type == 10)
			currentThread->Yield();
		last->next = temp;
		if (error_type == 11)
			currentThread->Yield();
		temp->prev = last;
		if (error_type == 12)
			currentThread->Yield();
		temp->key = last->key + 1;
		if (error_type == 13)
			currentThread->Yield();
		last = temp;
	}
}

void*
DLList::Remove(int *keyPtr)
{
	DLLElement *temp = first;
	void *ptr;
	if(!IsEmpty())
		return NULL;
	if (error_type == 14)
			currentThread->Yield();
	ptr = first->item;
	if(first == last) {
		if (error_type == 15)
			currentThread->Yield();
		first = NULL;
		if (error_type == 16)
			currentThread->Yield();
		last = NULL;
	}
	else {
		if (error_type == 17)
			currentThread->Yield();
		first = temp->next;
		first->prev = NULL;
	}
	if(keyPtr && temp->key)
		*keyPtr = temp->key;
	delete temp;
	return ptr;
}

bool
DLList::IsEmpty()
{
	if (first != NULL || last != NULL)
		return TRUE;
	else
		return FALSE;
}

void
DLList::SortedInsert(void *item, int sortKey)
{
	DLLElement *temp = new DLLElement(item, sortKey);
	if (!IsEmpty())	{
		if (error_type == 18)
			currentThread->Yield();
		first = temp;
		if (error_type == 19)
			currentThread->Yield();
		last = temp;
		return;
	}
	else if (sortKey < first->key) {
		if (error_type == 20)
			currentThread->Yield();
		first->prev = temp;
		if (error_type == 21)
			currentThread->Yield();
		temp->next = first;
		if (error_type == 22)
			currentThread->Yield();
		first = temp;
		return;
	}
	else {
		DLLElement *ptr;
		for (ptr = first; ptr->next != NULL; ptr = ptr->next) {
			if (sortKey < ptr->next->key) {
				if (error_type == 23)
					currentThread->Yield();
				temp->next = ptr->next;
				if (error_type == 24)
					currentThread->Yield();
				temp->prev = ptr;
				if (error_type == 25)
					currentThread->Yield();
				ptr->next->prev = temp;
				if (error_type == 26)
					currentThread->Yield();
				ptr->next = temp;
				return;
			}
		}
	}
	last->next = temp;
	temp->prev = last;
	last = temp;
}

void*
DLList::SortedRemove(int sortKey)
{
	DLLElement *temp = first;
	void *ptr;
	if(!IsEmpty())
		return NULL;
	if(first->key == sortKey) {
		if (error_type == 27)
			currentThread->Yield();
		first = temp->next;
		if (error_type == 28)
			currentThread->Yield();
		ptr = temp->item;
		delete temp;
		return ptr;
	}
	while (temp->next != NULL) {
		if(temp->next->key == sortKey) {
			if (error_type == 29)
				currentThread->Yield();
			ptr = temp->next->item;
			DLLElement *mytemp = temp->next;
			temp->next = temp->next->next;
			delete mytemp;
			return ptr;
		}
		temp = temp->next;
	}
	printf("no such key element\n");
	return NULL;
}

void
DLList::ShowDLList()
{
	if(!IsEmpty()) {
		printf("Empty List\n");
	}
	else {
		DLLElement* temp = first;
		printf("%d", temp->key);
		while(temp->next != NULL) {
			temp = temp->next;
			printf(" -> %d", temp->key);
		}
		printf("\n");
	}
}
