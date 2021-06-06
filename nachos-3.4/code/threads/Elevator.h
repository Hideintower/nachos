#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "system.h"
#include "EventBarrier.h"
#include "dllist.h"

#define UP 1
#define STOP 0
#define DOWN -1
/*

Here are the method signatures for the Elevator and Building classes.
You should feel free to add to these classes, but do not change the
existing interfaces.

*/

class Elevator {
   public:
     Elevator(char *debugName, int numFloors, int myID);
     ~Elevator();
     char *getName() { return name; }
   
     // elevator control interface: called by Elevator thread
     void OpenDoors();                //   signal exiters and enterers to action
     void CloseDoors();               //   after exiters are out and enterers are in
     void VisitFloor(int floor);      //   go to a particular floor
   
     // elevator rider interface (part 1): called by rider threads.
     bool Enter();                    //   get in
     void Exit();                     //   get out (iff destinationFloor)
     void RequestFloor(int floor);    //   tell the elevator our destinationFloor
   
     // insert your methods here, if needed
     void StartElevator();

   private:
     char *name;
     int currentfloor;           // floor where currently stopped
     int occupancy;              // how many riders currently onboard
     int totalfloor;
     int elevatorID;
     int maxsize;
     EventBarrier* exitBarrier;
     int direction;
     bool* exitWanted;
     bool isOverload;
     // insert your data structures here, if needed
};



class Building {
   public:
     Building(char *debugname, int numFloors, int numElevators);
     ~Building();
     char *getName() { return name; }
   
   				
     // elevator rider interface (part 2): called by rider threads
     void CallUp(int fromFloor);      //   signal an elevator we want to go up
     void CallDown(int fromFloor);    //   ... down
     Elevator *AwaitUp(int fromFloor); // wait for elevator arrival & going up
     Elevator *AwaitDown(int fromFloor); // ... down
     bool IsCallUp(int floor);
     bool IsCallDown(int floor);
     void SetElevatorUp(int floor, int ElevatorID);
     void SetElevatorDown(int floor, int ElevatorID);
     void SetCallUp(int floor, bool request);
     void SetCallDown(int floor, bool request);
     void RecordCall(int floor, int mydirection);
     int HandleCall();
     void HandleFloor(int floor, int mydirection);
   
   public:
     char *name;
     Elevator **elevator;         // the elevators in the building (array)
     int totalfloor;
     int totalelevator;
     bool* upWanted;
     bool* downWanted;
     int* ElevatorUp;
     int* ElevatorDown;
     EventBarrier* upBarrier;
     EventBarrier* downBarrier;
     int* WaitList;
     int ListTail;
     Condition* building_cond;
     // insert your data structures here, if needed
};

   // here's a sample portion of a rider thread body showing how we
   // expect things to be called.
   //
   // void rider(int id, int srcFloor, int dstFloor) {
   //    Elevator *e;
   //  
   //    if (srcFloor == dstFloor)
   //       return;
   //  
   //    DEBUG('t',"Rider %d travelling from %d to %d\n",id,srcFloor,dstFloor);
   //    do {
   //       if (srcFloor < dstFloor) {
   //          DEBUG('t', "Rider %d CallUp(%d)\n", id, srcFloor);
   //          building->CallUp(srcFloor);
   //          DEBUG('t', "Rider %d AwaitUp(%d)\n", id, srcFloor);
   //          e = building->AwaitUp(srcFloor);
   //       } else {
   //          DEBUG('t', "Rider %d CallDown(%d)\n", id, srcFloor);
   //          building->CallDown(srcFloor);
   //          DEBUG('t', "Rider %d AwaitDown(%d)\n", id, srcFloor);
   //          e = building->AwaitDown(srcFloor);
   //       }
   //       DEBUG('t', "Rider %d Enter()\n", id);
   //    } while (!e->Enter()); // elevator might be full!
   //  
   //    DEBUG('t', "Rider %d RequestFloor(%d)\n", id, dstFloor);
   //    e->RequestFloor(dstFloor); // doesn't return until arrival
   //    DEBUG('t', "Rider %d Exit()\n", id);
   //    e->Exit();
   //    DEBUG('t', "Rider %d finished\n", id);
   // }

#endif

