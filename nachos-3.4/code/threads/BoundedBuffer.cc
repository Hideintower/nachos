#include "BoundedBuffer.h"
#include "system.h"

BoundedBuffer::BoundedBuffer(int maxsize)
{
	bound_size = maxsize;
	buffer_lock = new Lock("buffer lock");
	buffer_empty = new Condition("buffer empty");
	buffer_full = new Condition("buffer full");
	buffer = new int[bound_size];
	rest_space= bound_size;
	head = 0;
	tail = 0;
}

BoundedBuffer::~BoundedBuffer()
{
	delete[] buffer;
	delete buffer_full;
	delete buffer_empty;
	delete buffer_lock;
}

void
BoundedBuffer::Read(void *data, int size)
{
	buffer_lock->Acquire();
	for(int i=0; i<size; i++)
	{
		while(rest_space == bound_size)
			buffer_empty->Wait(buffer_lock);
		*((int *)data+i) = buffer[head];
		head = (head + 1) % bound_size;
		rest_space++;
		currentThread->Yield();
		printf(" Read: head %d, tail %d, content %c\n",head, tail, *((char*)data+i));
		buffer_full->Signal(buffer_lock);
	}
	buffer_lock->Release();
}

void
BoundedBuffer::Write(void *data, int size)
{
	buffer_lock->Acquire();
	for(int i=0; i<size; i++)
	{
		while(rest_space == 0)
			buffer_full->Wait(buffer_lock);
		buffer[tail] = *((int *)data+i);
		currentThread->Yield();
		tail = (tail+1) % bound_size;
		rest_space--;
		printf("Write: head %d, tail %d, content %c\n",head, tail, *((char*)data+i));
		buffer_empty->Signal(buffer_lock);
	}
	buffer_lock->Release();
}
