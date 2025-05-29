#include "main.h"
#include "hbox.h"

static void hw_feed()
{

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
void  hwatchdog_init(const hruntime_function_t *func)
{
    hwatchdog_set_hardware_dog_feed(hw_feed);
    hwatchdog_setup_software_dog(sys_reset,sys_tick_ms);
}
HRUNTIME_INIT_EXPORT(watchdog,0,hwatchdog_init,NULL);
#endif
#ifdef HRUNTIME_USING_LOOP_SECTION
void  hwatchdog_loop(const hruntime_function_t *func)
{

}
HRUNTIME_LOOP_EXPORT(watchdog,0,hwatchdog_loop,NULL);
#endif

