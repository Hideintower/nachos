#include "Alarm.h"
#include "system.h"

extern Alarm* myalarm;

void Dummy_thread(int nouse)
{
	while(myalarm->getSleep() != 0)
		currentThread->Yield();
	currentThread->Finish();
}

Alarm::Alarm()
{
	thread_list = new DLList();
	sleep_num = 0;
}

Alarm::~Alarm()
{
	delete thread_list;
}

void
Alarm::Pause(int howlong)	//微秒级单位，转换为秒
{
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
	ASSERT(howlong>0);
	sleep_num++;
	if (!thread_list->IsEmpty())
	{
		Thread* dummy = new Thread("dummy_thread");
		dummy->Fork(Dummy_thread, (int)this);
	}
	int when = stats->totalTicks + howlong * TimerTicks * 100000;
	thread_list->SortedInsert((void*)currentThread, when);
	currentThread->Sleep();
	(void) interrupt->SetLevel(oldLevel);
}

void
Alarm::Wake_up()
{
	IntStatus oldLevel = interrupt->SetLevel(IntOff);
	int keyptr;
	Thread* alarmthread = (Thread*)thread_list->Remove(&keyptr);
	while(alarmthread)
	{
		if(keyptr <= stats->totalTicks)
		{
			scheduler->ReadyToRun(alarmthread);
			sleep_num--;
			alarmthread = (Thread*)thread_list->Remove(&keyptr);
			continue;
		}
		else
		{
			thread_list->SortedInsert((void*)alarmthread, keyptr);
			break;
		}
	}

	(void) interrupt->SetLevel(oldLevel);
}

int
Alarm::getSleep()
{
	return sleep_num;
}
