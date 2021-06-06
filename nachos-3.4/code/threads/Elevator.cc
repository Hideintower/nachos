#include "Elevator.h"

extern int capacity;	//-1无限大
extern Building* building;
extern int error_type;
Lock elevator_lock("elevator lock");

Elevator::Elevator(char* debugname, int numFloors, int myID)
{
	name = debugname;
	totalfloor = numFloors;
	elevatorID = myID;
	switch(elevatorID%2)
	{
		case 0:
			currentfloor = 0;
			break;
		case 1:
			currentfloor = totalfloor - 1;
			break;
	}
	occupancy = 0;
	maxsize = capacity;
	exitBarrier = new EventBarrier[totalfloor];
	direction = STOP;
	isOverload = FALSE;
	exitWanted = new bool[totalfloor];
	for(int i=0; i<totalfloor; i++)
	{
		exitWanted[i] = FALSE;
	}
}

Elevator::~Elevator()
{
	delete[] exitBarrier;
	delete[] exitWanted;
}

void
Elevator::OpenDoors()
{
	printf("$$ Elevator %d Open the Door at Floor %d\n", elevatorID, currentfloor);
	if(exitWanted[currentfloor])
	{
		exitBarrier[currentfloor].Signal();
		exitWanted[currentfloor] = FALSE;
	}
	if(building->IsCallUp(currentfloor+1) && (direction == UP || direction == STOP))
	{
		building->SetElevatorUp(currentfloor+1, elevatorID);
		building->upBarrier[currentfloor].Signal();
		building->HandleFloor(currentfloor+1, UP);
		building->SetCallUp(currentfloor+1, isOverload);
	}
	else if(building->IsCallDown(currentfloor+1) && (direction == DOWN || direction == STOP))
	{
		building->SetElevatorDown(currentfloor+1, elevatorID);
		building->downBarrier[currentfloor].Signal();
		building->HandleFloor(currentfloor+1, DOWN);
		building->SetCallDown(currentfloor+1, isOverload);
	}
}

void
Elevator::CloseDoors()
{
	printf("$$ Elevator %d Close the Door at Floor %d\n", elevatorID, currentfloor+1);
	if(occupancy == 0)
	{
		direction = STOP;
		printf("$$ Elevator %d Stop at Floor %d\n", elevatorID, currentfloor+1);
	}
	else if(currentfloor == 0 && direction == DOWN)
	{
		direction = UP;
		printf("$$ Elevator %d Change the Direction at Floor %d\n", elevatorID, currentfloor+1);
	}
	else if(currentfloor == totalfloor-1 && direction == UP)
	{
		direction = DOWN;
		printf("$$ Elevator %d Change the Direction at Floor %d\n", elevatorID, currentfloor+1);
	}
}

void
Elevator::VisitFloor(int floor)
{
	printf("$$ Elevator %d goes to visit Floor %d\n", elevatorID, floor);
	currentfloor = floor-1;
	if((!isOverload && (direction == UP && building->IsCallUp(currentfloor+1)) || (direction == DOWN && building->IsCallDown(currentfloor+1))) || exitWanted[currentfloor])
	{
		OpenDoors();
		currentThread->Yield();
		CloseDoors();
	}
}

void
Elevator::StartElevator()
{
	while(TRUE)
	{
		elevator_lock.Acquire();
		if(occupancy == 0)
		{
			int request = building->HandleCall();
			if(!request)
			{
				switch(elevatorID%2)
				{
					case 0:
						currentfloor = 0;
						break;
					case 1:
						currentfloor = totalfloor - 1;
						break;
				}
				direction = STOP;
				printf("Elevator %d is Stop at %d\n", elevatorID, currentfloor+1);
				building->building_cond->Wait(&elevator_lock);
				printf("Elevator %d Start to Work at %d\n", elevatorID, currentfloor+1);
				elevator_lock.Release();
			}
			else
			{
				if(request>0)
				{
					currentfloor = request-1;
					direction = UP;
				}
				else
				{
					currentfloor = -request+1;
					direction = DOWN;
				}
				VisitFloor(currentfloor+1);
				elevator_lock.Release();
				currentThread->Yield();
			}
		}
		else
		{
			if(direction == UP)
			{
				for(int i=currentfloor+1; i<totalfloor; i++)
				{
					if(exitWanted[i] || building->IsCallUp(i+1))
					{
						VisitFloor(i+1);
						break;
					}
				}
			}
			else if(direction == DOWN)
			{
				for(int i=currentfloor; i>=0; i--)
				{
					if(exitWanted[i] || building->IsCallDown(i+1))
					{
						VisitFloor(i+1);
						break;
					}
				}
			}
			elevator_lock.Release();
			currentThread->Yield();
		}
	}
}

bool
Elevator::Enter()
{
	if(maxsize == -1 || !isOverload)
	{
		occupancy++;
		if(occupancy == maxsize)
			isOverload = TRUE;
		if(direction == UP)
		{
			building->upBarrier[currentfloor].Complete();
		}
		else
		{
			building->downBarrier[currentfloor].Complete();
		}
		return TRUE;
	}
	else
	{
		if(direction == UP)
		{
			building->upBarrier[currentfloor].Complete();
		}
		else
		{
			building->downBarrier[currentfloor].Complete();
		}
		return FALSE;
	}
}

void
Elevator::Exit()
{
	occupancy--;
	exitBarrier[currentfloor].Complete();
}

void
Elevator::RequestFloor(int floor)
{
	exitWanted[floor-1] = TRUE;
	exitBarrier[floor-1].Wait();
}

void
Initialize_Elevator(int id)
{
	building->elevator[id] = new Elevator("building elevator", building->totalfloor, id);
	building->elevator[id]->StartElevator();
}

Building::Building(char* debugname, int numFloors, int numElevators)
{
	name = debugname;
	totalfloor = numFloors;
	totalelevator = numElevators;
	elevator = new Elevator*[totalelevator];
	upWanted = new bool[totalfloor];
	downWanted = new bool[totalfloor];
	ElevatorUp = new int[totalfloor];
	ElevatorDown = new int[totalfloor];
	upBarrier = new EventBarrier[totalfloor];
	downBarrier = new EventBarrier[totalfloor];
	WaitList = new int[1024];
	ListTail = 0;
	building_cond = new Condition("condition for wake up elevator");
	for(int i=0; i<numElevators; i++)
	{
		upWanted[i] = FALSE;
		downWanted[i] = FALSE;
		ElevatorUp[i] = -1;
		ElevatorDown[i] = -1;
		Thread* t = new Thread("building elevator thread");
		t->Fork(Initialize_Elevator, i);
	}
}

Building::~Building()
{
	for(int i=0; i<totalelevator; i++)
		delete elevator[i];
	delete[] elevator;
	delete[] upWanted;
	delete[] downWanted;
	delete[] upBarrier;
	delete[] downBarrier;
	delete[] WaitList;
	delete building_cond;
}

void
Building::CallUp(int fromFloor)
{
	upWanted[fromFloor-1] = TRUE;
	RecordCall(fromFloor, UP);
	elevator_lock.Acquire();
	building_cond->Broadcast(&elevator_lock);
	elevator_lock.Release();
}

void
Building::CallDown(int fromFloor)
{
	downWanted[fromFloor-1] = TRUE;
	RecordCall(fromFloor, DOWN);
	elevator_lock.Acquire();
	building_cond->Broadcast(&elevator_lock);
	elevator_lock.Release();
}

Elevator*
Building::AwaitUp(int fromFloor)
{
	upBarrier[fromFloor-1].Wait();
	int id = ElevatorUp[fromFloor-1];
	return elevator[id];
}

Elevator*
Building::AwaitDown(int fromFloor)
{
	downBarrier[fromFloor-1].Wait();
	int id = ElevatorDown[fromFloor-1];
	return elevator[id];
}

bool
Building::IsCallUp(int floor)
{
	return upWanted[floor-1];
}

bool
Building::IsCallDown(int floor)
{
	return downWanted[floor-1];
}

void
Building::SetElevatorUp(int floor, int ElevatorID)
{
	ElevatorUp[floor-1] = ElevatorID;
}

void
Building::SetElevatorDown(int floor, int ElevatorID)
{
	ElevatorDown[floor-1] = ElevatorID;
}

void
Building::SetCallUp(int floor, bool request)
{
	upWanted[floor-1] = request;
}

void 
Building::SetCallDown(int floor, bool request)
{
	downWanted[floor-1] = request;
}

void
Building::RecordCall(int floor, int mydirection)
{
	WaitList[ListTail++] = floor * mydirection;
}

int 
Building::HandleCall()
{
	int id;
	if(ListTail > 0)
	{
		id = WaitList[0];
		ListTail--;
		for(int i=0; i<ListTail; i++)
		{
			WaitList[i] = WaitList[i+1];
		}
	}
	else
	{
		id = 0;
	}
	return id;
}

void 
Building::HandleFloor(int floor, int mydirection)
{
	int i, j;
	for(i=0; i<ListTail; i++)
	{
		if(WaitList[i] == floor * mydirection)
		{
			WaitList[i] = 0;
		}
	}
	for(i=0, j=0; i<ListTail; i++)
	{
		if(WaitList[i])
		{
			WaitList[j++] = WaitList[i];
		}
	}
	ListTail = j;
}

void rider(int id, int srcFloor, int dstFloor)
{
	Elevator* e;

	if(srcFloor == dstFloor)
	{
		return;
	}

	printf("Rider %d travelling from %d to %d\n", id, srcFloor, dstFloor);
	do
	{
		if(srcFloor < dstFloor)
		{
			printf("Rider %d CallUp(%d)\n", id, srcFloor);
			building->CallUp(srcFloor);
			printf("Rider %d AwaitUp(%d)\n", id, srcFloor);
			e = building->AwaitUp(srcFloor);
		}
		else
		{
			printf("Rider %d CallDown(%d)\n", id, srcFloor);
            building->CallDown(srcFloor);
            printf("Rider %d AwaitDown(%d)\n", id, srcFloor);
            e = building->AwaitDown(srcFloor);
		}
		DEBUG('t', "Rider %d Enter()\n", id);
	} while (!e->Enter());
	printf("Rider %d RequestFloor(%d)\n", id, dstFloor);
    e->RequestFloor(dstFloor);
    printf("Rider %d Exit()\n", id);
    e->Exit();
    printf("Rider %d finished\n", id);
}
