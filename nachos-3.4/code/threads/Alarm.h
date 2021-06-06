#ifndef ALARM_H
#define ALARM_H

#include "dllist.h"
#include "system.h"

class Alarm{
public:
	Alarm();
	~Alarm();
	/*
	whose interval has expired. There is no requirement 
	that an awakened thread starts running immediately 
	after the interval expires; just put them on the 
	ready queue after they have waited for at least 
	the specified interval ( howLong ). We have not created 
	a header file for Alarm , so you may define the rest 
	of the class interface as you see fit. You may use 
	any convenient unit for howLong .
	*/
	void Pause(int howlong);
	void Wake_up();
	int getSleep();
	friend void Dummy_thread(int nouse);
private:
	DLList *thread_list;
	int sleep_num;
};

#endif
