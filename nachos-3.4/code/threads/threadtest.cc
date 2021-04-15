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

extern void Initialize();
extern void Insert(int thread_num, int insert_num, DLList* list);
extern void TRemove(int keyptr, int thread_num, int remove_num,  DLList *list);
extern void TestPrepend(void *item, DLList* list);
extern void TestAppend(void* item, DLList* list);
extern void TestSortedRemove(int sortKey, DLList* list);

// testnum is set in main.cc
int testnum = 2;
extern int error_type;
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
    }
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

void
SimpleSynchTest()
{
    
    return;
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
    Initialize();
    for( int i=0; i<testnum; i++){
        Thread *t = new Thread("forked thread");
        t->Fork(SimpleThread, i);
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

