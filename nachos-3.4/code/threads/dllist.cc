#include "dllist.h"
#include "copyright.h"
#include "utility.h"

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
		first = temp;
		last = temp;
		temp->key = 1;
	}
	else {
		first->prev = temp;
		temp->next = first;
		temp->key = first->key - 1;
		first = temp;
	}
}

void
DLList::Append(void *item)
{
	DLLElement *temp = new DLLElement(item, 0);
	if (!IsEmpty()) {
		first = temp;
		last = temp;
		temp->key = 1;
	}
	else {
		last->next = temp;
		temp->prev = last;
		temp->key = last->key + 1;
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
	ptr = first->item;
	if(first == last) {
		first = NULL;
		last = NULL;
	}
	else {
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
		first = temp;
		last = temp;
		return;
	}
	else if (sortKey < first->key) {
		first->prev = temp;
		temp->next = first;
		first = temp;
		return;
	}
	else {
		DLLElement *ptr;
		for (ptr = first; ptr->next != NULL; ptr = ptr->next) {
			if (sortKey < ptr->next->key) {
				temp->next = ptr->next;
				temp->prev = ptr;
				ptr->next->prev = temp;
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
		first = temp->next;
		ptr = temp->item;
		delete temp;
		return ptr;
	}
	while (temp->next != NULL) {
		if(temp->next->key == sortKey) {
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
