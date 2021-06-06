#include "Table.h"
#include "system.h"

Table::Table(int size)
{
	table_size = size;
	table_lock = new Lock("table lock");
	mytable = new void*[table_size];
}

Table::~Table()
{
	delete[] mytable;
	delete table_lock;
}

int
Table::Alloc(void* object)
{
	table_lock->Acquire();
	currentThread->Yield();
	for(int i=0; i<table_size; i++)
	{
		if(!mytable[i])
		{
			mytable[i] = object;
			table_lock->Release();
			return i;
		}
	}
	table_lock->Release();
	return -1;
}

void*
Table::Get(int index)
{
	ASSERT(index < table_size && index >= 0);
	currentThread->Yield();
	return mytable[index];
}

void
Table::Release(int index)
{
	table_lock->Acquire();
	ASSERT(index < table_size && index >= 0);
	currentThread->Yield();
	mytable[index] = NULL;
	table_lock->Release();
}
