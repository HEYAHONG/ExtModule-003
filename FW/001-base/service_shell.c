#include "service_shell.h"
#include "main.h"
#include "hbox.h"
#include "drv_uart.h"
#include "drv_psram.h"


#ifndef SERVICE_SHELL_PRIORITY_LEVEL
#define SERVICE_SHELL_PRIORITY_LEVEL    16
#endif
#if (PRODUCT_SHELL_ENABLE) > 0
static uint8_t console_rx_buffer[256]= {0};
static hringbuf_t * console_rx_buffer_get(void)
{
    hringbuf_t * buffer=hringbuf_get(console_rx_buffer,sizeof(console_rx_buffer));
    //不加锁
    hringbuf_set_lock(buffer,NULL,NULL,NULL);
    return buffer;
}

static void uart_rx_handler(uint8_t *data,size_t length)
{
    hringbuf_t * buffer= console_rx_buffer_get();
    hringbuf_input(buffer,data,length);
}
#endif

int hbox_shell_putchar(int ch)
{
    if(ch>0)
    {
#if ((PRODUCT_CONSOLE_ENABLE) > 0) || ((PRODUCT_SHELL_ENABLE) > 0)
        uint8_t data=ch;
        uart1_tx(&data,sizeof(data),5);
#endif
    }
    return ch;
}

int hbox_shell_getchar(void)
{
    int ch=EOF;
    {
#if (PRODUCT_SHELL_ENABLE) > 0
        hringbuf_t * buffer= console_rx_buffer_get();
        if(hringbuf_get_length(buffer))
        {
            uint8_t ch_val=0;
            hringbuf_output(buffer,&ch_val,sizeof(ch_val));
            ch=ch_val;
        }
#endif

    }
    return ch;
}


#ifdef HRUNTIME_USING_INIT_SECTION
#if (PRODUCT_CONSOLE_ENABLE) > 0
static void putchar_cb(char c)
{
    hbox_shell_putchar((uint8_t)c);
}
#endif
void  hbox_shell_init(const hruntime_function_t *func)
{
#if (PRODUCT_CONSOLE_ENABLE) > 0
    hprintf_set_callback(putchar_cb);
#endif
#if (PRODUCT_SHELL_ENABLE) > 0
    uart1_set_rx_handler(uart_rx_handler);
    hshell_command_name_shortcut_set(NULL,true);
    HSHELL_COMMANDS_REGISTER(NULL);
#endif
    console_printf("console init!");
    console_printf("psram: %d kbytes",psram_size()/1024);
}
HRUNTIME_INIT_EXPORT(product,SERVICE_SHELL_PRIORITY_LEVEL,hbox_shell_init,NULL);
#endif
#ifdef HRUNTIME_USING_LOOP_SECTION

void  hbox_shell_loop(const hruntime_function_t *func)
{
#if (PRODUCT_SHELL_ENABLE) > 0
    while(hshell_loop(NULL)==0);
#endif
}
HRUNTIME_LOOP_EXPORT(product,SERVICE_SHELL_PRIORITY_LEVEL,hbox_shell_loop,NULL);
#endif

#if (PRODUCT_SHELL_ENABLE) > 0

/*
 * 系统复位
 */
static int cmd_reboot_entry(int argc,const char *argv[])
{
    (void)argc;
    (void)argv;
    NVIC_SystemReset();
    return 0;
}
HSHELL_COMMAND_EXPORT(reboot,cmd_reboot_entry,reboot system);

#endif