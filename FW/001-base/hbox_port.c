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

#ifdef HFPUTS
int HFPUTS( const char* str, FILE* stream )
{
    (void)str;
    (void)stream;
    return EOF;
}
#endif

#ifdef HVFPRINTF
int HVFPRINTF(FILE* stream,const char* format,va_list vlist)
{
    (void)stream;
    (void)format;
    (void)vlist;
    return EOF;
}
#endif

#ifdef HPUTS
int HPUTS( const char* str)
{
    (void)str;
    return EOF;
}
#endif

