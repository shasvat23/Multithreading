/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "stdafx.h"
#include "ResourceLock.h"


_hResourceLock_Sem resourceLock_create(const char *name)
{
    sem_t *sem_id = sem_open(name,O_CREAT,0600,0);
    
    if(sem_id == SEM_FAILED)
    {
        printf("\nFailed to open semaphore %s", name);
        return NULL;
    }
    
    sem_unlink(name);
    sem_post(sem_id);
    return (_hResourceLock_Sem)sem_id;
    
    
}
BOOL resourceLock_get(_hResourceLock_Sem h, unsigned uiMilliseconds)
{
    if(!h)
        return FALSE;
    
    sem_t *sem_id = (sem_t *)h;
    
    struct timespec ts; 
    if(clock_gettime(CLOCK_REALTIME, &ts)== -1)
    {
        return FALSE; 
    }
    
    ts.tv_nsec += (uiMilliseconds*1000000);        
    ts.tv_sec += ts.tv_nsec / 1000000000;
    ts.tv_nsec %= 1000000000;
    
    
    int i = sem_timedwait(sem_id,&ts);
    //int i = sem_timewait(sem_id);
    if(i<0)
    {
        return FALSE;
    }
    
    return TRUE;
    
}
BOOL resourceLock_release(_hResourceLock_Sem h)
{
    if(!h)
        return FALSE; 
    
    sem_t *sem_id = (sem_t*)h;
    
    int i; 
    
    i = sem_post(sem_id);
    if (i < 0)
        return FALSE; 
    //i = sem_close(sem_id);
    
    return TRUE;
    
    
}
BOOL resourceLock_try(_hResourceLock_Sem h)
{
    
}