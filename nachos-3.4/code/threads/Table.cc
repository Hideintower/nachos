#include "Table.h"
#include "synch.h"

Table::Table(int size)
{
	table_size = size;
	table_list = new DLList();
}

Table::~Table()
{
	delete table_list;
}

int
Table::Alloc(void* object)
{

}

void*
Table::Get(int index)
{

}

void
Table::Release(int index)
{

}
