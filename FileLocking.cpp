/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "FileLocking.h"

static ResourceFileLock *me = NULL; 

static void timeout_handler(int sig)
{
    
    me->timeout_expired =1; 
}    
    
 ResourceFileLock::ResourceFileLock()
{
     fd= -1; 
     timeout_expired = 0; 
     me = this;
     
}
 
 ResourceFileLock::~ResourceFileLock()
 {
     
 }
 
 
 BOOL ResourceFileLock::createLock(char *FileName)
 {
     if(!FileName)
         return FALSE; 
     strncpy(fName,FileName,strlen(FileName));
     fd =  open(fName, O_CREAT|O_RDWR, S_IRWXU|S_IRWXG|S_IRWXO);
     if(fd < 0)
         return FALSE; 
     
     return TRUE;
            
 }
BOOL ResourceFileLock::acquireLock(int msTimeout)
{
    if(fd <= 0)
        return FALSE;
    
    
    struct itimerval timeout, old_timer; 
    
    struct sigaction sa, old_sa; 
    
    int err; 
    
    int sTimeout = msTimeout/1000; 
    
    memset(&timeout,0,sizeof(timeout));
    
    timeout.it_value.tv_sec = sTimeout; 
    timeout.it_value.tv_usec = ((msTimeout-(sTimeout*1000))*1000); 
    
    memset(&sa, 0, sizeof(sa)); 
    
    sa.sa_handler = timeout_handler; 
    sa.sa_flags = SA_RESETHAND; 
    sigaction(SIGALRM, &sa, &old_sa); 
    setitimer(ITIMER_REAL, &timeout, &old_timer); 
    
    
    int cntTimeout =0;    
    
    
    while(flock(fd,LOCK_EX))
    {
        switch((err = errno)){
            case EINTR : 
                if(timeout_expired){
                    setitimer(ITIMER_REAL, &old_timer, NULL);
                    sigaction(SIGALRM,&old_sa, NULL); 
                    return FALSE;
                }
                else
                    continue; 
            default:
                return FALSE; 
                
        }
    }
    
    setitimer(ITIMER_REAL, &old_timer,NULL);
    sigaction(SIGALRM, &old_sa, NULL); 
    
    return TRUE;   
    
    
}

BOOL ResourceFileLock::releaseLock()
{
    flock(fd, LOCK_UN);    
    return TRUE;
}


BOOL ResourceFileLock::deleteLock()
{
    close(fd);
    return TRUE;
}