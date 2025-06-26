#ifndef __DRV_UART_H__
#define __DRV_UART_H__
#include "stdint.h"
#include "stdlib.h"
#include "main.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

UART_HandleTypeDef * uart1_get_handle(void);
UART_HandleTypeDef * uart2_get_handle(void);

typedef void (*uart_rx_handler_t)(uint8_t *data,size_t length);
void uart1_set_rx_handler(uart_rx_handler_t handler);
void uart2_set_rx_handler(uart_rx_handler_t handler);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif
