#include "drv_uart.h"
#include "drv_led.h"
#include "drv_psram.h"
#include "hbox.h"
#include "boot.h"


#ifndef APP_MAIN_PRIORITY_LEVEL
#define APP_MAIN_PRIORITY_LEVEL    48
#endif
size_t console_buffer_size(void);
static void hprintf_putchar_cb(char c)
{
    hputchar((uint8_t)c);
}

hminirv32ima_machine_embed_t vm= {};
static bool hminirv32ima_machine_embed_console_has_data(const struct hminirv32ima_machine_embed*machine)
{
    return console_buffer_size();
}
static void hminirv32ima_machine_embed_console_put_data(const struct hminirv32ima_machine_embed*machine,uint8_t data)
{
    hputchar(data);
}
static uint8_t hminirv32ima_machine_embed_console_get_data(const struct hminirv32ima_machine_embed*machine)
{
    return hgetchar();
}
static size_t    hminirv32ima_machine_embed_mem_load_psram(const struct hminirv32ima_memory *mem,uintptr_t ram_addr,void *ptr,size_t length)
{
    psram_read(ram_addr-mem->ram_base,(uint8_t *)ptr,length);
    return length;
}
static size_t    hminirv32ima_machine_embed_mem_store_psram(const struct hminirv32ima_memory *mem,uintptr_t ram_addr,const void *ptr,size_t length)
{
    psram_write(ram_addr-mem->ram_base,(const uint8_t *)ptr,length);
    return length;
}
static bool vm_init(void)
{
    memset(&vm,0,sizeof(vm));
    hminirv32ima_machine_embed_init(&vm);
    vm.console_has_data=hminirv32ima_machine_embed_console_has_data;
    vm.console_put_data=hminirv32ima_machine_embed_console_put_data;
    vm.console_get_data=hminirv32ima_machine_embed_console_get_data;
    vm.mem_size=psram_size();
    if(vm.mem_size < 1*1024*1024)
    {
        return false;
    }
    vm.mem_load=hminirv32ima_machine_embed_mem_load_psram;
    vm.mem_store=hminirv32ima_machine_embed_mem_store_psram;
    hminirv32ima_machine_embed_reset(&vm);

    return true;
}


static bool  is_vm_init_ok=true;
#ifdef HRUNTIME_USING_INIT_SECTION
static void  happ_main_init(const hruntime_function_t *func)
{

    hprintf_set_callback(hprintf_putchar_cb);

    if(!vm_init())
    {
        led_set_sys_mask(0);
        is_vm_init_ok=false;
    }
}
HRUNTIME_INIT_EXPORT(app_main,APP_MAIN_PRIORITY_LEVEL,happ_main_init,NULL);
#endif

enum
{
    VM_STATE_INIT=0,
    VM_STATE_LOAD,
    VM_STATE_RUNNING,
};
static uint8_t vm_state=VM_STATE_INIT;

#ifdef HRUNTIME_USING_LOOP_SECTION
static void  happ_main_loop(const hruntime_function_t *func)
{
    if(is_vm_init_ok)
    {
        switch(vm_state)
        {
        case VM_STATE_INIT:
        {
            hminirv32ima_machine_embed_reset(&vm);
            BootInit();
            vm_state=VM_STATE_LOAD;
            led_set_sys_mask(0xFFFFFFFF);
            hprintf("vm:mem_size= %d bytes\r\n",vm.mem_size);
        }
        break;
        case VM_STATE_LOAD:
        {
            if(NvmVerifyChecksum())
            {
                vm_state=VM_STATE_RUNNING;
                led_set_sys_mask(0xF0F0F0F0);
            }
            else
            {
                BootTask();
            }
        }
        break;
        case VM_STATE_RUNNING:
        {
            static hdefaults_tick_t core_tick=0;
            hdefaults_tick_t current_tick=hdefaults_tick_get();
            int code=hminirv32ima_machine_embed_step(&vm,(current_tick-core_tick)*1000,4096);
            core_tick=current_tick;
            if(code != 0 && code != 1)
            {
                //非正常状态(包括重启及关机)均回归初始化状态
                vm_state=VM_STATE_INIT;
            }
        }
        break;
        default:
        {

        }
        break;
        }
    }
}
HRUNTIME_LOOP_EXPORT(app_main,APP_MAIN_PRIORITY_LEVEL,happ_main_loop,NULL);
#endif

