#include "main.h"
#include "hbox.h"
#include "drv_psram.h"
#include "drv_uart.h"


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

static uint64_t console_rx_buffer[384/sizeof(uint64_t)]= {0};
static hringbuf_t * console_rx_buffer_get(void)
{
    hringbuf_t * buffer=hringbuf_get((uint8_t *)console_rx_buffer,sizeof(console_rx_buffer));
    hringbuf_set_lock(buffer,NULL,NULL,NULL);
    return buffer;
}

size_t console_buffer_size(void)
{
    return hringbuf_get_length(console_rx_buffer_get());
}

static void uart_rx_handler(uint8_t *data,size_t length)
{
    hringbuf_t * buffer= console_rx_buffer_get();
    hringbuf_input(buffer,data,length);
}


#ifdef HPUTCHAR

int  HPUTCHAR(int ch)
{
    uart1_set_rx_handler(uart_rx_handler);
    uint8_t data=ch;
    uart1_tx(&data,sizeof(data),10);
    return ch;
}

#endif

#ifdef HGETCHAR

int  HGETCHAR(void)
{
    if(console_buffer_size() > 0)
    {
        uint8_t ch=0;
        hringbuf_output(console_rx_buffer_get(),&ch,sizeof(ch));
        return ch;
    }
    return -1;
}

#endif


