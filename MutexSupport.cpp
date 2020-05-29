/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "stdafx.h"
#include <unistd.h>
#include <string.h>
#include "MutexSupport.h"
using namespace std;
__uint64_t GetTickCount()
{
    
    std::chrono::milliseconds uptime(0u); 
    __uint64_t uptime_seconds;

    if (std::ifstream("/proc/uptime", std::ios::in) >> uptime_seconds) 
    { 
       // uptime = std::chrono::milliseconds( static_cast<unsigned long long>(uptime_seconds)*1000ULL ); 
        uptime_seconds = (uptime_seconds)*1000ULL ;
    }
    
    return uptime_seconds;
    
    
}
_hMutex mutex_create(void)
{
	pthread_mutex_t *m; 
	m = (pthread_mutex_t*)calloc(1, sizeof(*m));

	pthread_mutex_init(m, NULL);
	//  pthread_mutex_lock(m);
	return (_hMutex)m;
}

void mutex_delete(_hMutex hm)
{
	pthread_mutex_destroy((pthread_mutex_t*)hm);
	free(hm);
}

unsigned Millitime_OS_Support(void)  
{
	static unsigned t0=0;
	unsigned t1 = GetTickCount();
	if(!t0)
		t0 = t1;
	
	return t1 - t0;
	
}

BOOL mutex_get(_hMutex m)
{
	int i = pthread_mutex_trylock((pthread_mutex_t*)m);
        if(i==0)
            return TRUE;
        else
        {    printf("Failed to acquire mutex \n");
            return FALSE;
        }
        
        return TRUE;
}

int mutex_try_lock(_hMutex m)
{
  int i =  pthread_mutex_trylock((pthread_mutex_t*)m);
  if(i==0)
      return TRUE; 
  return FALSE;
}
// dwMilliseconds=0 -> do not waid;  dwMilliseconds = INFINITE, see WinBase.h, which defines INFINITE to be DWORD(-1) 
void mutex_release(_hMutex m)
{
   int j = mutex_try_lock(m);
#if 0
   if(j != 0)
       msg("Failed to try lock mutex before release ", errno);
#endif
   int i = pthread_mutex_unlock((pthread_mutex_t*)m);
   if(i != 0)
        printf("Failed to unlock mutex before release \n");
}


//////////////////////////////////////////////////////////////////////////
// Un-Named Pipes
//////////////////////////////////////////////////////////////////////////


_hPipe pipe_create(void)
{
	pipeData *p;
	p = (pipeData *)calloc(1, sizeof(*p));
	p->write_mutex = mutex_create();

	// Recall: int pipe(int fd[2]);

	int fd[2];
	pipe(fd);
	p->read_fd = fd[0];
	p->write_fd = fd[1];
	return (_hPipe)p;
}

void pipe_delete(_hPipe ap)
{
	pipeData *p = (pipeData *)ap;
	if(!p)
		return;
	mutex_delete(p->write_mutex);
	close(p->read_fd);
	close(p->write_fd);
	free(p);
}

BOOL pipeWrite(_hPipe ap, const void *buf, int sz)	// This will be multi-writer mutex protected
{
	pipeData *p = (pipeData *)ap;
	mutex_get(p->write_mutex);

	int szWritten = write(p->write_fd, buf, sz);

	mutex_release(p->write_mutex);

	return (szWritten == sz);
}

int  pipeRead(_hPipe ap,  void *buf, int maxSz) // I assume single-readers
{
	pipeData *p = (pipeData *)ap;
	//int numByteRead=0;
	int numByteRead = read(p->read_fd, buf, maxSz);

	return numByteRead;
}


//////////////////////////////////////////////////////////////////////////
// Thread Priorities -- 
//////////////////////////////////////////////////////////////////////////
int GetMaxThreadPrioirtyForFIFO()
{
        struct sched_param Sp;
	memset(&Sp, 0, sizeof(Sp));
	int policy = SCHED_FIFO;
	int maxPri = sched_get_priority_max(policy);
        return maxPri;
}
BOOL setThreadPriRt(int pri,pthread_t thread_id)	// "pri" is 0 <lesser priority> to ?? 4 ?? <highest priority>
{
        int iret=0;

	struct sched_param Sp;
	memset(&Sp, 0, sizeof(Sp));
	int policy = SCHED_FIFO;        
	int maxPri = sched_get_priority_max(policy);
	//int tpri = maxPri - 4 + pri;
        int tpri = min(maxPri,pri);
        int minpri = sched_get_priority_min(policy);
        if(tpri == minpri)
            tpri += 4;
	Sp.sched_priority = tpri;

         
        
        iret = pthread_setschedparam(thread_id, policy, &Sp);
	if (iret == 0)
		return 1;
        
	printf("Could not set thread priority, error %d \n", errno);
	return 0;
}
BOOL setThreadPriTimeShare(int pri)
{

	struct sched_param Sp;
	memset(&Sp, 0, sizeof(Sp));
	int policy = SCHED_RR;
	int maxPri = sched_get_priority_max(policy);
	int tpri = maxPri - 4 + pri;
	Sp.sched_priority = tpri;

	pthread_t thread_id = pthread_self();
	int iret = pthread_setschedparam(thread_id, policy, &Sp);
	if (iret == 0)
		return 1;
	printf("Could not set thread priority \n");
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Cpu-Binding
//////////////////////////////////////////////////////////////////////////
BOOL setThreadAffinity(unsigned cpuMask)
{
	cpu_set_t S;
	CPU_ZERO(&S);
	for (int k = 0; k < 32; k++)
	{
		if (cpuMask & (0x1 << k))
			CPU_SET(k, &S);	// zero-based cpu id's, see man7.org CPU_SET
	}

	pthread_t thread_id = pthread_self();
	int i = pthread_setaffinity_np(thread_id, sizeof(S), &S);
        if(i != 0)
            printf("Failed to set process affinity,errno %d \n ",errno);
        
	return 1;
}

BOOL setProcessAffinity(__pid_t pid, int cpuAffinity)
{
    
    if (cpuAffinity > -1)
    {
        cpu_set_t mask;
        int status;

        CPU_ZERO(&mask);
        CPU_SET(cpuAffinity, &mask);
        status = sched_setaffinity(pid, sizeof(mask), &mask);
        if (status == 0)
        {
            return 1;
        }
        return 0;
    }
}