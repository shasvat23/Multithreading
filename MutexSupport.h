/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MutexSupport.h
 * Author: VIBE-USER
 *
 * Created on April 27, 2020, 7:11 PM
 */

#pragma once 
#include<pthread.h>
#include<stdio.h>

#define TRUE 1
#define FALSE 0

#ifndef MUTEXSUPPORT_H
#define MUTEXSUPPORT_H

typedef void * _hMutex;

_hMutex mutex_create(void);
void mutex_delete(_hMutex hm);
#ifdef __linux__
BOOL mutex_get(_hMutex myMutex);

#else
BOOL mutex_get(_hMutex m, unsigned dwMilliseconds);	
// dwMilliseconds=0 -> do not waid;  dwMilliseconds = INFINITE, see WinBase.h, which defines INFINITE to be DWORD(-1) 
#endif
void mutex_release(_hMutex m);
int mutex_try_lock(_hMutex m);



typedef void * _hPipe;
typedef struct _pipeInternals
{

	int write_fd;
	int read_fd;
	_hMutex write_mutex;
} pipeData;



_hPipe pipe_create(void);
void pipe_delete(_hPipe ap);
BOOL pipeWrite(_hPipe ap, const void *buf, int sz);	// This will be multi-writer mutex protected
int  pipeRead(_hPipe ap,  void *buf, int maxSz); // I assume single-readers

//////////////////////////////////////////////////////////////////////////
// Thread Priorities -- 
//////////////////////////////////////////////////////////////////////////
int GetMaxThreadPrioirtyForFIFO();
BOOL setThreadPriRt(int pri,pthread_t thread_id);// "pri" is 0 <lesser priority> to ?? 4 ?? <highest priority>
BOOL setThreadPriTimeShare(int pri);


//////////////////////////////////////////////////////////////////////////
// Cpu-Binding
//////////////////////////////////////////////////////////////////////////
BOOL setThreadAffinity(unsigned cpuMask);


BOOL setProcessAffinity(__pid_t pid, int cpuAffinity);



#endif /* MUTEXSUPPORT_H */

