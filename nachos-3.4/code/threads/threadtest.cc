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

extern void Insert(int thread_num, int insert_num, DLList* list);
extern void TRemove(int keyptr, int thread_num, DLList *list);
extern void TestPrepend(void *item, DLList* list);
extern void TestAppend(void* item, DLList* list);
extern void TestSortedRemove(int sortKey, DLList* list);

// testnum is set in main.cc
int testnum = 1;
DLList* list = new DLList();

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
    int num, key;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
    Insert(which, num+1, list);
    list->ShowDLList();
    TRemove(key, which, list);
    list->ShowDLList();
        //currentThread->Yield();
    }
    printf("after prepend:\n");
    TestPrepend(NULL, list);
    printf("after append:\n");
    TestAppend(NULL, list);
    printf("after sorted remove:\n");
    TestSortedRemove(33, list);
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

    Thread *t = new Thread("forked thread");

    //t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}

