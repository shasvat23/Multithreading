///*
// * To change this license header, choose License Headers in Project Properties.
// * To change this template file, choose Tools | Templates
// * and open the template in the editor.
// */
//
///* 
// * File:   main.cpp
// * Author: shasha
// *
// * Created on November 30, 2018, 12:41 AM
// */
//
////
//#define _GNU_SOURCE
//#include <assert.h>
//#include <sched.h>
//#include <stdbool.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//
//void print_affinity() {
//    cpu_set_t mask;
//    long nproc, i;
//
//    if (sched_getaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
//        perror("sched_getaffinity");
//        assert(false);
//    } else {
//        nproc = sysconf(_SC_NPROCESSORS_ONLN);
//        printf("sched_getaffinity = ");
//        for (i = 0; i < nproc; i++) {
//            printf("%d ", CPU_ISSET(i, &mask));
//        }
//        printf("\n");
//    }
//}
//
//int main(void) {
//    cpu_set_t mask;
//
//    print_affinity();
//    printf("sched_getcpu = %d\n", sched_getcpu());
//    CPU_ZERO(&mask);
//    CPU_SET(0, &mask);
//    if (sched_setaffinity(0, sizeof(cpu_set_t), &mask) == -1) {
//        perror("sched_setaffinity");
//        assert(false);
//    }
//    print_affinity();
//    /* TODO is it guaranteed to have taken effect already? Always worked on my tests. */
//    printf("sched_getcpu = %d\n", sched_getcpu());
//    return EXIT_SUCCESS;
//}


#include<sched.h>
#include<pthread.h>
#include<semaphore.h>
#include <unistd.h> 
#include<thread>
#include <sys/types.h>
#include<iostream>
#include<condition_variable>
#include<mutex>
#include<libudev.h>

#include "SemaphoreEvents.h"
#include "MutexSupport.h"

using namespace std;
sem_t emptyBuffers,fullBuffers,oneEmptyBuffer, oneFullBuffer; 
std::condition_variable cv; 
std::mutex m; 
bool quitReq = 0;
#define NUM_TOTAL_BUFFERS 5 
#define DATA_LENGTH 20 
static int counter = 0 ; 
static char PrepareData(void)
{
   // sleep(0.5);
    char val = rand() % 26; 
    val += 'A';
    return val;
}
static void ProcessData()
{
    sleep(0.5);
}
bool isFull()
{
  return (counter == NUM_TOTAL_BUFFERS )   ;
}

bool isEmpty()
{
    return (counter < NUM_TOTAL_BUFFERS );
}

void write_func(void *buffers)
{
    unique_lock<&m> lock; 
    cv.wait(&lock,[]{return !isFull();}); 
    char *buf = (char *)buffers;
    int i, writePt = 0; 
    char data ; 
    for (i =0; i< DATA_LENGTH;i++)
    {
        data = PrepareData(); 
        buf[writePt]= data; 
        cout<< pthread_self() << " buffer["<<writePt<<"] = "<< data <<endl;
        writePt = (writePt +1) % NUM_TOTAL_BUFFERS;
        
    }
    cv.notify_all();
}

void * writter_func (void *buffers)
{
    char *buf = (char *)buffers;
    int i, writePt = 0; 
    char data ; 
    
    for (i =0; i< DATA_LENGTH;i++)
    {   
        sleep(1);
        data = PrepareData(); 
        sem_wait(&emptyBuffers);
        buf[writePt]= data; 
        
        cout<< pthread_self() << " buffer["<<writePt<<"] = "<< data <<endl;
        writePt = (writePt +1) % NUM_TOTAL_BUFFERS;
        sem_post(&fullBuffers);
               //sleep(1);
    }
    if(i == DATA_LENGTH)
        quitReq = 1;
}

void * reader_func (void *buffers)
{
    char *buf = (char *)buffers;
    int readPt =0; 
    char data ; 
    
    struct timespec ts;
    while(!quitReq)
   {
//    for (int i =0; i< DATA_LENGTH;i++)
//    {
       // sem_wait(&fullBuffers);
        
            clock_gettime(CLOCK_REALTIME, &ts);
            ts.tv_sec += 1;
            sem_timedwait(&fullBuffers,&ts);
            if(errno == ETIMEDOUT )
            {
                cout<<"semaphore timedout"<<endl; 
            
            }
        
            data = buf[readPt];
            cout<< pthread_self() << " buffer["<<readPt<<"] = "<< data <<endl;
            readPt = (readPt + 1)% NUM_TOTAL_BUFFERS;
            sem_post(&emptyBuffers);
            ProcessData();
        
//       }
   }
}
void * onewritter_func(void *buffer)
{
    bool quitreq = 0; 
    char *buf = (char *)buffer;
    int i, writePt = 0; 
    char data,key ; 
    struct timespec ts;
    for(int i =0; i< 10; i++)
    {
        data = PrepareData(); 
        cout<<"Data prepared = "<<data<<endl;
        sleep(1);
        sem_post(&oneFullBuffer);    
        *buf = data;
        ts.tv_sec += 1;
        sem_timedwait(&oneFullBuffer,&ts);
        if(errno == ETIMEDOUT)
        {
            cout<<"single read semaphore timeout"<< endl; 
        }
//        else
//        {
//            quitreq = 1;
//        }
        
    }
}
void * onereader_func(void *buffer)
{
    bool quitreq = 0; 
     char *buf = (char *)buffer;
    int readPt =0; 
    char data ;
    struct timespec ts;
    sleep(15);
    while(!quitreq)
    {
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;
        sem_timedwait(&oneFullBuffer,&ts);
        if(errno == ETIMEDOUT )
        {
            cout<<"single semaphore timedout"<<endl; 
            
        }
//        else
//        {
//            quitreq = 1;
//        }
        data = buf[0]; 
        cout<< pthread_self() << " buffer = "<< data <<endl;
        ProcessData();
    }
    
}

WinStyleEvents *Event ; 

void * classSemaphorewritter_func(void *buffer)
{
    bool quitreq = 0; 
    char *buf = (char *)buffer;
    int i, writePt = 0; 
    char data,key ; 
    struct timespec ts;
    for(int i =0; i< 10; i++)
    {
        data = PrepareData(); 
        cout<<"Data prepared = "<<data<<endl;
        sleep(1);
        Event->SetEvent();    
        *buf = data;
        ts.tv_sec += 1;
        Event->WaitForSingleObject(1000);
        if(errno == ETIMEDOUT)
        {
            cout<<"single read semaphore timeout"<< endl; 
        }
//        else
//        {
//            quitreq = 1;
//        }
        
    }
}
void * classSemaphorereader_func(void *buffer)
{
    bool quitreq = 0; 
     char *buf = (char *)buffer;
    int readPt =0; 
    char data ;
    struct timespec ts;
    sleep(15);
    while(!quitreq)
    {   
        BOOL b = Event->CheckEvent();
        if(!b)
            printf("Checkevent failed \n");
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;
        Event->WaitForSingleObject(1000);
        if(errno == ETIMEDOUT )
        {
            cout<<"single semaphore timedout"<<endl; 
            continue;
            
        }
//        else
//        {
//            quitreq = 1;
//        }
        data = buf[0]; 
        cout<< pthread_self() << " buffer = "<< data <<endl;
        ProcessData();
    }
    
}
#include <cerrno>
#include <cstring>

    pthread_t writer, reader, one_writer, one_reader, class_writter, class_reader ; 
    char buffers[NUM_TOTAL_BUFFERS];
    char bufff;
    char onebuffer;

        
int main(int argc, char**argv) 
{
//            sem_init(&emptyBuffers, 0,NUM_TOTAL_BUFFERS);
//    sem_init(&fullBuffers,0,0);
//    pthread_create (&writer, NULL,writter_func,buffers);
//    pthread_create (&reader, NULL,reader_func, buffers); 
//    pthread_join(writer, NULL);
//    pthread_join(reader, NULL);
//    sem_destroy(&emptyBuffers);
//    sem_destroy(&fullBuffers);
      
 /////////onewrtter/////////////////////////////////////////////// 
//    sem_init(&oneFullBuffer,0,1);
//    pthread_create (&one_writer, NULL,onewritter_func,&buffers);
//    pthread_create (&one_reader, NULL,onereader_func, &buffers); 
//    pthread_join(one_writer, NULL);
//    pthread_join(one_reader, NULL);
//    printf("All done \n");    
//    sem_destroy(&oneFullBuffer);
////////////////////////////////////////////////////////////////////
    
    
/////////class semaphore///////////////////////////////////////////
    Event = new WinStyleEvents("/Evnt");
    Event->CreateEvent();
   // Event.InitEventObject("/Evnt1");
    pthread_create (&class_writter, NULL,classSemaphorewritter_func,&buffers);
    pthread_create (&class_reader, NULL,classSemaphorereader_func, &buffers); 
    pthread_join(class_writter, NULL);
    pthread_join(class_reader, NULL);
    printf("All done \n");    
////////////////////////////////////////////////////////////////////
    
//        struct sched_param params; 
//     // We'll set the priority to the maximum.
//        params.sched_priority = sched_get_priority_max(SCHED_FIFO)-4;
//        
//        
//        int ret;
//        int policy = SCHED_FIFO;
//        pthread_t this_thread = pthread_self();
//        ret = pthread_setschedparam(this_thread, policy, &params);
//        if (ret != 0) 
//     {
//         std::cout <<  std::strerror(errno)<<endl;
//        
//     }
//        ret = pthread_getschedparam(this_thread, &policy, &params);
//     if (ret != 0) 
//     {
//         std::cout << "Couldn't retrieve real-time scheduling paramers" << std::endl;
//        
//     }
    //command to execute : g++-8 welcome.cc -std=c++17 -no-pie -lpthread
 
  return 0;
}

