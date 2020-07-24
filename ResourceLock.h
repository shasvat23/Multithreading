/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ResourceLock.h
 * Author: SCP
 *
 * Created on July 24, 2020, 2:27 PM
 */

#ifndef ResourceLock
#define ResourceLock

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <sys/time.h>
#include <signal.h>



typedef int _ResourceLockHandle;
_ResourceLockHandle Create_ResourceLock(char *lockFile) ;
BOOL Get_ResourceLock(_ResourceLockHandle h, int msTimeout) ; //create and gets resource lock if does not exist, else if exist just gets it if available
BOOL Release_ResourceLock (_ResourceLockHandle lockFd) ;

#endif /* ResourceLock */

