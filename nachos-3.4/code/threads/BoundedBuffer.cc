#include "BoundedBuffer.h"
#include "synch.h"

BoundedBuffer::BoundedBuffer(int maxsize)
{
	bound_size = maxsize;
}

BoundedBuffer::~BoundedBuffer()
{

}

void
BoundedBuffer::Read(void *data, int size)
{

}

void
BoundedBuffer::Write(void *data, int size)
{
	
}
