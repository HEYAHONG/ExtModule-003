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

#ifdef HABORT
void HABORT()
{
    NVIC_SystemReset();
}
#endif

#ifdef HEXIT
void HEXIT(int exit_code)
{
    (void)exit_code;
    habort();
}
#endif

#ifdef HFWRITE
size_t HFWRITE(const void*buffer,size_t obj_size,size_t obj_count,FILE * stream )
{
    (void)buffer;
    (void)obj_size;
    (void)obj_count;
    (void)stream;
    return 0;
}
#endif



