#include "main.h"
#include "hbox.h"


#ifdef HDEFAULTS_TICK_GET
hdefaults_tick_t HDEFAULTS_TICK_GET()
{
    return HAL_GetTick();
}
#endif

static int lock_nested=0;
#ifdef HDEFAULTS_MUTEX_LOCK
void HDEFAULTS_MUTEX_LOCK()
{
    if(lock_nested==0)
    {
        __disable_irq();
    }
    lock_nested++;
}
#endif

#ifdef HDEFAULTS_MUTEX_UNLOCK
void HDEFAULTS_MUTEX_UNLOCK()
{
    lock_nested--;
    if(lock_nested==0)
    {
        __enable_irq();
    }
}
#endif

