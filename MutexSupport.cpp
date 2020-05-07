/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "stdafx.h"
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