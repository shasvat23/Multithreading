/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ResourceLock.h
 * Author: VIBE-USER
 *
 * Created on July 20, 2020, 1:38 PM
 */

#ifndef RESOURCELOCK_H
#define RESOURCELOCK_H

#include "stdafx.h"
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


typedef void * _hResourceLock_Sem; 

_hResourceLock_Sem resourceLock_create(const char *name);
BOOL resourceLock_get(_hResourceLock_Sem h, unsigned uiMilliseconds);
BOOL resourceLock_release(_hResourceLock_Sem h);
BOOL resourceLock_try(_hResourceLock_Sem h);

#endif /* RESOURCELOCK_H */

