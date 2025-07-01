#include "app_main.h"
#include "drv_uart.h"
#include "service_wireless.h"

#ifndef APP_MAIN_PRIORITY_LEVEL
#define APP_MAIN_PRIORITY_LEVEL    48
#endif

#if (PRODUCT_UART2_BOARDCAST) > 0
__USED
static void app_uart_rx_handler(uint8_t *data,size_t length)
{
    service_wireless_frame_boardcast_transmit(data,length);
};

__USED
static void service_wireless_frame_boardcast_rx_handler(uint8_t *data,size_t datalen)
{
    uart2_tx(data,datalen,datalen+5);
}
#endif


#ifdef HRUNTIME_USING_INIT_SECTION
static void  happ_main_init(const hruntime_function_t *func)
{
#if (PRODUCT_UART2_BOARDCAST) > 0
    /*
     * 使用uart2透传广播数据帧
     */
    uart2_set_rx_handler(app_uart_rx_handler);
    service_wireless_frame_boardcast_set_rx_handler(service_wireless_frame_boardcast_rx_handler);
#endif
	
}
HRUNTIME_INIT_EXPORT(app_main,APP_MAIN_PRIORITY_LEVEL,happ_main_init,NULL);
#endif
#ifdef HRUNTIME_USING_LOOP_SECTION
static void  happ_main_loop(const hruntime_function_t *func)
{

}
HRUNTIME_LOOP_EXPORT(app_main,APP_MAIN_PRIORITY_LEVEL,happ_main_loop,NULL);
#endif

