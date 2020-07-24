#include "ResourceLock.h"
static sig_atomic_t timeout_expired = 0;
//************************************************************

static void timeout_handler(int sig)
{
  (void)sig;

  timeout_expired = 1;
}

_ResourceLockHandle Create_ResourceLock(char *lockFile) 
{
       _ResourceLockHandle lockFd = open(lockFile, O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO)  ;
       
        return lockFd;
}

//************************************************************
BOOL Get_ResourceLock(_ResourceLockHandle lockFd, int msTimeout) 
{
    struct itimerval timeout, old_timer;
    struct sigaction sa, old_sa;
    int err;
    int sTimeout = msTimeout/1000;
    memset(&timeout, 0, sizeof timeout);

    timeout.it_value.tv_sec = sTimeout;
    timeout.it_value.tv_usec = ((msTimeout-(sTimeout*1000))*1000);

    memset(&sa, 0, sizeof sa);

    sa.sa_handler = timeout_handler;
    sa.sa_flags   = SA_RESETHAND;
    sigaction(SIGALRM, &sa, &old_sa);
    setitimer(ITIMER_REAL, &timeout, &old_timer);


 
    int cntTimeout = 0;

    

    while (flock(lockFd, LOCK_EX))
    {
        switch( (err = errno) ) {
            case EINTR:         /* Signal received */
                if ( timeout_expired )
                    setitimer(ITIMER_REAL, &old_timer, NULL); /* Cancel itimer */
                    sigaction(SIGALRM, &old_sa, NULL); /* Cancel signal handler */
                    return FALSE;      /* -w option set and failed to lock */
                continue;           /* otherwise try again */
            default:            /* Other errors */
                return FALSE;  
        }
    }   

    setitimer(ITIMER_REAL, &old_timer, NULL); /* Cancel itimer */
    sigaction(SIGALRM, &old_sa, NULL); /* Cancel signal handler */

    return TRUE;
}
//***************************************************************
BOOL Release_ResourceLock (_ResourceLockHandle lockFd) 
{
    flock(lockFd, LOCK_UN);
    
    return TRUE;
}

BOOL Delete_ResourceLock (_ResourceLockHandle lockFd)
{    
    close(lockFd);
    return TRUE;
}

//************************************************************