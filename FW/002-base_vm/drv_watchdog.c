#include "main.h"
#include "hbox.h"

#ifndef DRV_WATCHDOG_PRIORITY_LEVEL
#define DRV_WATCHDOG_PRIORITY_LEVEL    0
#endif

static IWDG_HandleTypeDef   IwdgHandle= {0};
static void hw_feed()
{
    HAL_IWDG_Refresh(&IwdgHandle);
};
static void sys_reset()
{
    NVIC_SystemReset();
}
static hwatchdog_tick_t sys_tick_ms()
{
    return hdefaults_tick_get();
};

#ifdef HRUNTIME_USING_INIT_SECTION
static void  hwatchdog_init(const hruntime_function_t *func)
{
    hwatchdog_set_hardware_dog_feed(hw_feed);
    hwatchdog_setup_software_dog(sys_reset,sys_tick_ms);
    IwdgHandle.Instance = IWDG;                        /* IWDG */
    IwdgHandle.Init.Prescaler = IWDG_PRESCALER_256;    /* Prescaler DIV 256 */
    IwdgHandle.Init.Reload = (1024);                   /* IWDG Reload value 1024, 1024/(32768/256)=8 */
    HAL_IWDG_Init(&IwdgHandle);
    __HAL_DBGMCU_FREEZE_IWDG();                        /* can be removed in the product */

}
HRUNTIME_INIT_EXPORT(watchdog,DRV_WATCHDOG_PRIORITY_LEVEL,hwatchdog_init,NULL);
#endif
#ifdef HRUNTIME_USING_LOOP_SECTION
static void  hwatchdog_loop(const hruntime_function_t *func)
{

}
HRUNTIME_LOOP_EXPORT(watchdog,DRV_WATCHDOG_PRIORITY_LEVEL,hwatchdog_loop,NULL);
#endif

