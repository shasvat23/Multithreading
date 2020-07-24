/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileLocking.h
 * Author: SCP
 *
 * Created on July 24, 2020, 10:58 AM
 */

#ifndef FILELOCKING_H
#define FILELOCKING_H

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <sys/time.h>
#include <signal.h>


class ResourceLock{
    
public:
ResourceLock();
~ResourceLock();
BOOL createLock(char *FileName);    
BOOL acquireLock(int msTimeout);
BOOL releaseLock(); 
BOOL deleteLock();
sig_atomic_t timeout_expired ;
int fd; 

private : 
    
     
    
    char fName[256];
    
};


#endif /* FILELOCKING_H */

