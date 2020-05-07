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

#endif /* MUTEXSUPPORT_H */

