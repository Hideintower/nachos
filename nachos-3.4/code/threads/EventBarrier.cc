#include "EventBarrier.h"

EventBarrier::EventBarrier()
{
	wait_thread = 0;
	is_barried = TRUE;
	barrier_lock = new Lock("barrier lock");
	wait_for = new Condition("wait for other threads");
	wake_up = new Condition("wake up other threads");
}

EventBarrier::~EventBarrier()
{
	delete wait_for;
	delete wake_up;
	delete barrier_lock;
}

void
EventBarrier::Wait()
{
	barrier_lock->Acquire();
	wait_thread++;
	if(!is_barried)
	{
		barrier_lock->Release();
		return;
	}
	wait_for->Wait(barrier_lock);
	barrier_lock->Release();
}

void
EventBarrier::Signal()
{
	barrier_lock->Acquire();
	is_barried = FALSE;
	wait_for->Broadcast(barrier_lock);
	while(wait_thread)
	{
		wake_up->Wait(barrier_lock);
	}
	barrier_lock->Release();
}

void
EventBarrier::Complete()
{
	barrier_lock->Acquire();
	wait_thread--;
	if(wait_thread == 0)
	{
		is_barried = TRUE;
		wake_up->Broadcast(barrier_lock);
	}
	else
	{
		wake_up->Wait(barrier_lock);
	}
	barrier_lock->Release();
}

int
EventBarrier::Waiters()
{
	return wait_thread;
}

bool
EventBarrier::Status()
{
	return is_barried;
}

