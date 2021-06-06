// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "dllist.h"
#include "synch.h"
#include "Table.h"
#include "BoundedBuffer.h"
#include "Alarm.h"
#include "EventBarrier.h"
#include "Elevator.h"


extern void Initializer();
extern void Insert(int thread_num, int insert_num, DLList* list);
extern void TRemove(int keyptr, int thread_num, int remove_num,  DLList *list);
extern void TestPrepend(void *item, DLList* list);
extern void TestAppend(void* item, DLList* list);
extern void TestSortedRemove(int sortKey, DLList* list);
extern void rider(int id, int srcFloor, int dstFloor);
extern int randnum(int num, int rest);

// testnum is set in main.cc
int testnum = 2;
int floornum = 10;
int elevatornum = 2;
int capacity = -1;
int error_type = 0;
DLList* list = new DLList();
//Lock* newlock = new Lock("test Lock");
BoundedBuffer* test_buffer = new BoundedBuffer(8);
Table* test_table = new Table(4);

EventBarrier* mybarrier = new EventBarrier();
Alarm* myalarm = new Alarm();
Building* building;


void to_test_table()
{
    char insert_message[] = "what an amazing thing!";
    char* get_message;
    get_message = (char*)test_table->Get(3);
    if(get_message)
        printf("%s\n", get_message);
    else
        printf("Nothing\n");
    int pos = test_table->Alloc((void*)insert_message);
    get_message = (char*)test_table->Get(pos);
    if(get_message)
        printf("%s\n", get_message);
    else
        printf("Nothing\n");
    test_table->Release(pos);
    get_message = (char*)test_table->Get(pos);
    if(get_message)
        printf("%s\n", get_message);
    else
        printf("Nothing\n");
}


void to_test_buffer(int which)
{
    char insert_message[] = "what an amazing thing!";
    char get_message[24];
    switch(which%2){
        case 0:
            test_buffer->Read((void*)get_message, sizeof(get_message));
            if(get_message)
                printf("%s\n", get_message);
            else
                printf("Nothing\n");
            break;
        case 1:
            test_buffer->Write((void*)insert_message, sizeof(insert_message));
            printf("insert complete\n");
            break;
    }
}

void to_test_barrier(int which)
{
    if(which == testnum)
    {
        printf("***Control Thread %d\n", which);
        /*
        while(mybarrier->Waiters() != testnum)
            currentThread->Yield();
        */
        mybarrier->Signal();
        printf("***Control Thread Signaled\n");
    }
    else
    {
        printf("***Work Thread %d\n", which);
        printf("***Work Thread %d Start to Wait, Now waiting %d\n", which, mybarrier->Waiters());
        mybarrier->Wait();
        //currentThread->Yield();
        mybarrier->Complete();
        printf("***Work Thread %d Completed\n", which);
    }
}

void to_test_alarm(int which)
{
    printf("***Alarm Thread %d Enters\n", which);
    int howlong = randnum(10, 1);
    printf("***Alarm Thread %d, Current Time:%d, Alarm Time:%d\n", which, stats->totalTicks, howlong * TimerTicks * 100000+stats->totalTicks);
    myalarm->Pause(howlong);
    printf("***Alarm Thread %d, Current Time:%d\n", which, stats->totalTicks);
}

void to_test_elevator(int which)
{
    int srcFloor, dstFloor;
    do
    {
        srcFloor = randnum(floornum, 1);
        dstFloor = randnum(floornum, 1);
    }while (srcFloor == dstFloor);
    rider(which, srcFloor, dstFloor);
}

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    /*
    Lock* thread_lock;
    Condition* newcondition = new Condition("test Condition");
    thread_lock = new Lock("thread lock");
    newcondition->Wait(thread_lock);
    newlock->Acquire();
    */
    int num, key;
    switch(error_type) {
        case 0:
            for (num = 0; num < 4; num++) {
                printf("*** thread %d looped %d times\n", which, num);
                Insert(which, num+1, list);
                list->ShowDLList();
                TRemove(key, which, num/2+1, list);
                list->ShowDLList();
                currentThread->Yield();
            }
            break;
        case 1:
            switch(which % 2) {
                case 0:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        Insert(which, num+1, list);
                        list->ShowDLList();
                        if (num == 1);
                            currentThread->Yield();
                    }
                    break;
                case 1:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        TRemove(key, which, num+1, list);
                        list->ShowDLList();
                        //currentThread->Yield();
                    }
                    break;
            }
            break;
        case 2:
            // 错误制造同下
        case 3:
            switch(which % 2) {
                case 0:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        TestPrepend(NULL, list);
                        //currentThread->Yield();
                    }
                    break;
                case 1:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        Insert(which, num+1, list);
                        list->ShowDLList();
                        currentThread->Yield();
                    }
                    break;
            }
            break;
        case 4:

        case 5:

        case 6:

        case 7:
            switch(which % 2) {
                case 0:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        TestPrepend(NULL, list);
                        //currentThread->Yield();
                    }
                    break;
                case 1:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        TRemove(key, which, 1, list);
                        list->ShowDLList();
                        currentThread->Yield();
                    }
                    break;
            }
            break;
        case 8:

        case 9:

        case 10:

        case 11:

        case 12:

        case 13:
            switch(which % 2) {
                case 0:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        TestAppend(NULL, list);
                        //currentThread->Yield();
                    }
                    break;
                case 1:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        Insert(which, num+1, list);
                        list->ShowDLList();
                        currentThread->Yield();
                    }
                    break;
            }
            break;
        case 14:

        case 15:

        case 16:

        case 17:
            switch(which % 2) {
                case 0:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        Insert(which, num+1, list);
                        list->ShowDLList();
                        TRemove(key, which, 1, list);
                        list->ShowDLList();
                        //currentThread->Yield();
                    }
                    break;
                case 1:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        TestPrepend(NULL, list);
                        currentThread->Yield();
                    }
                    break;
            }
            break;
        case 18:
            switch(which % 2) {
                case 0:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        Insert(which, num+1, list);
                        list->ShowDLList();
                        currentThread->Yield();
                    }
                    break;
                case 1:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        TestPrepend(NULL, list);
                        currentThread->Yield();
                    }
                    break;
            }
            break;
        case 19:
            switch(which % 2) {
                case 0:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        Insert(which, num+1, list);
                        list->ShowDLList();
                        currentThread->Yield();
                    }
                    break;
                case 1:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        TestAppend(NULL, list);
                        currentThread->Yield();
                    }
                    break;
            }
            break;
        case 20:

        case 21:

        case 22:
            switch(which % 2) {
                case 0:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        Insert(which, num+1, list);
                        list->ShowDLList();
                        currentThread->Yield();
                    }
                    break;
                case 1:
                    for (num = 0; num < 4; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        TestPrepend(NULL, list);
                        currentThread->Yield();
                    }
                    break;
            }
            break;
        case 23:

        case 24:

        case 25:

        case 26:
            for (num = 0; num < 4; num++) {
                printf("*** thread %d looped %d times\n", which, num);
                Insert(which, num+1, list);
                list->ShowDLList();
                //currentThread->Yield();
            }
            break;
        case 27:

        case 28:

        case 29:
            switch(which % 2) {
                case 0:
                    for (num = 0; num < 5; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        Insert(which, num+1, list);
                        list->ShowDLList();
                        //currentThread->Yield();
                    }
                    break;
                case 1:
                    for (num = 0; num < 5; num++) {
                        printf("*** thread %d looped %d times\n", which, num);
                        TestSortedRemove(33, list);
                        //currentThread->Yield();
                    }
                    break;
            }
            break;
        case 30:
            printf("*** thread %d\n", which);
            to_test_table();
            to_test_buffer(which);
            break;
        case 31:
            printf("*** Thread %d Start\n", which);
            //DEBUG('t', "*** Thread %d Start\n", which);
            to_test_barrier(which);
            break;
        case 32:
            printf("***Thread %d Start\n", which);
            to_test_alarm(which);
            break;
        case 33:
            printf("***Thread %d Start\n", which);
            to_test_elevator(which);
            currentThread->Yield();
            break;
    }

    /*
    newlock->Release();
    newcondition->Signal(thread_lock);
    */

    /*
    int num, key;
    for (num = 0; num < 5; num++) {
	   printf("*** thread %d looped %d times\n", which, num);
        Insert(which, num+1, list);
        list->ShowDLList();
        TRemove(key, which, num/2+1, list);
        list->ShowDLList();
        currentThread->Yield();
    }
    printf("after prepend:\n");
    TestPrepend(NULL, list);
    printf("after append:\n");
    TestAppend(NULL, list);
    printf("after sorted remove:\n");
    TestSortedRemove(33, list);
    */
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");
    Initializer();
    if(error_type == 33)
        building = new Building("test building", floornum, elevatornum);
    for( int i=0; i<testnum; i++){
        Thread *t = new Thread("forked thread");
        t->Fork(SimpleThread, i);
    }
    if(error_type == 31)
    {
        Thread *t = new Thread("forked thread");
        t->Fork(SimpleThread, testnum);
    }
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 2:
	   ThreadTest1();
	break;
    default:
        ThreadTest1();
        //printf("No test specified.\n");
	break;
    }
}

