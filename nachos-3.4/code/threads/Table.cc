#include "Table.h"

Table::Table(int size)
{
	table_size = size;
	table_lock = new Lock("table lock");
	mytable = new void*[table_size];
}

Table::~Table()
{
	delete[] mytable;
}

int
Table::Alloc(void* object)
{
	table_lock->Acquire();
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
	return mytable[index];
}

void
Table::Release(int index)
{
	table_lock->Acquire();
	ASSERT(index < table_size && index >= 0);
	mytable[index] = NULL;
	table_lock->Release();
}
