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


typedef void * _hResourceLock; 

_hResourceLock resourceLock_create(const char *name);
BOOL resourceLock_get(_hResourceLock h, unsigned uiMilliseconds);
BOOL resourceLock_release(_hResourceLock h);
BOOL resourceLock_try(_hResourceLock h);

#endif /* RESOURCELOCK_H */

