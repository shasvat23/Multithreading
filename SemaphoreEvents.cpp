/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "stdafx.h"
#include "SemaphoreEvents.h"
#include "MutexSupport.h"
#include <semaphore.h>
#include <string.h>




void strNcpy(char *dst, const char *src, int max)
	{
		//ASSERT(src);
		//ASSERT(dst);
		strncpy(dst,src,max);
		dst[max -1] = 0;
	}

WinStyleEvents::WinStyleEvents(const char *name)
{
    EventMutex = mutex_create();
    if(!EventMutex)
        printf("Failed to init mutex for event \n"); 
    hsem = new _hSemEvent;
    temp = new _hSemEvent;
    strNcpy(hsem->name,name,sizeof(hsem->name));
    strNcpy(temp->name,name,sizeof(temp->name));
}

WinStyleEvents::~WinStyleEvents()
{
    mutex_delete(EventMutex);
    delete hsem;
}

BOOL WinStyleEvents::InitEventObject(const char *name)
{
    if(!EventMutex)
    {
        
        EventMutex = mutex_create();
        if(!EventMutex)
            printf("Failed to init mutex for event \n"); 
        hsem = new _hSemEvent;
        strNcpy(hsem->name,name,ARRAYSIZE(hsem->name));
        
        temp = new _hSemEvent;
        strNcpy(temp->name,name,ARRAYSIZE(temp->name));
    }
    
    return TRUE;
}


BOOL WinStyleEvents::CreateEvent() //pshared variable will allow if we want the semaphore to be shared between processes
                                          // if pshared is 0 then semaphore will only be shared between threads in a process
{
    if(!EventMutex)
    {
        EventMutex = mutex_create(); 
        if(!EventMutex)
        {
            printf("Failed to initialize mutex for events \n");
            return FALSE;
        }
    } 

    
    //We dont use pshared anymore
    //We are using named semaphores now
    //While opening a named semaphore we will tell who can read/write on it

    BOOL b = mutex_get(EventMutex);
    if(!b)
    {
        printf("Failed to acquire mutex during creating event \n");
        mutex_release(EventMutex);
        return FALSE;
    }
    
    
    hsem->sem= sem_open(hsem->name,O_CREAT ,S_IRUSR | S_IWUSR,1);
    if(hsem->sem == SEM_FAILED)
    {
        int i= sem_close(hsem->sem);
        if(!(i==0))
            printf("Previous sem could not be closed \n");
        hsem->sem= sem_open(hsem->name,O_CREAT ,S_IRUSR | S_IWUSR,1);
        
        if(hsem->sem == SEM_FAILED)
        {
            mutex_release(EventMutex);
            printf("Failed inside create event \n");
            return FALSE;
        }
    }
    sem_unlink(hsem->name);
    mutex_release(EventMutex);
    return TRUE;
}

BOOL WinStyleEvents::SetEvent()
{
    BOOL b = mutex_get(EventMutex);
    if(!b)
        return FALSE;
    
     int i  = sem_post(hsem->sem);
     if(i < 0)
     {
         mutex_release(EventMutex);
         return FALSE;
     }
     
    mutex_release(EventMutex);
    return TRUE;
}


BOOL WinStyleEvents::WaitForSingleObject(long double time_millsec)
{
    BOOL b = mutex_get(EventMutex);
    if(!b)
        return FALSE;
    struct timespec ts; 
    if(clock_gettime(CLOCK_REALTIME, &ts)== -1)
    {
        mutex_release(EventMutex);
        return FALSE; 
    }
    
    ts.tv_sec += (time_millsec/1000);
    int i = sem_timedwait(hsem->sem,&ts);
    if(i<0)
    {
        mutex_release(EventMutex);
        return FALSE;
    }
    mutex_release(EventMutex);
    return TRUE;
    
}
BOOL WinStyleEvents::DeleteEvent()
{
#if 0
    int i = sem_destroy(sem);
    if(i < 0)
        return FALSE; 
    return TRUE;
#endif
    BOOL b = mutex_get(EventMutex);
    if(!b)
        return FALSE;
    
    int i = sem_close(hsem->sem);
    if(i ==0)
    {
        hsem->sem =NULL;
        mutex_release(EventMutex);    
        return TRUE; 
    }
    
    mutex_release(EventMutex);
    return FALSE;
}


BOOL WinStyleEvents::CheckEvent()
{
#if 0
    int i,sval; 
    i = sem_getvalue(sem,&sval); //sval will contain current value of semaphore if it exist
    if(i < 0)
        return FALSE; 
    return TRUE;
#endif
    
#if 0
    BOOL b = mutex_get(EventMutex);
    if(!b)
    {
        printf("Failed to acquire mutex \n");
        return FALSE;
    }
    temp->sem = sem_open(hsem->name,O_CREAT | O_EXCL,S_IRUSR | S_IWUSR,1);
    if(temp->sem  == SEM_FAILED)
    {
        mutex_release(EventMutex);
        return TRUE; 
    }
    
    mutex_release(EventMutex);
    return FALSE;
#endif
    BOOL b = mutex_get(EventMutex);
    if(!b)
    {
        printf("Failed to acquire mutex \n");
        return FALSE;
    }
    if(hsem->sem)
    {
        mutex_release(EventMutex);
        return TRUE; 
    }
    
    mutex_release(EventMutex);
    return FALSE;
}







