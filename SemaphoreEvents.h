/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SemaphoreEvents.h
 * Author: VIBE-USER
 *
 * Created on April 27, 2020, 7:10 PM
 */

#ifndef SEMAPHOREEVENTS_H
#define SEMAPHOREEVENTS_H

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   WinStyleEventSupport.h
 * Author: VIBE-USER
 *
 * Created on April 23, 2020, 3:40 PM
 */

#pragma once 

#include "stdafx.h"
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "MutexSupport.h"

#ifndef WINSTYLEEVENTSUPPORT_H
#define WINSTYLEEVENTSUPPORT_H




typedef struct _SemEvent
{
    sem_t *sem; 
    char name[100];
}_hSemEvent;

class WinStyleEvents
{
    
private: 
    _hMutex EventMutex;
    _hSemEvent *hsem;
    _hSemEvent *temp;
public:
    WinStyleEvents(const char *name); 
    ~WinStyleEvents();
    
BOOL InitEventObject(const char *name);
BOOL CreateEvent() ; 
BOOL SetEvent();
BOOL WaitForSingleObject(long double time_millsec); 
BOOL DeleteEvent();
BOOL CheckEvent();
};


#endif /* WINSTYLEEVENTSUPPORT_H */



#endif /* SEMAPHOREEVENTS_H */

