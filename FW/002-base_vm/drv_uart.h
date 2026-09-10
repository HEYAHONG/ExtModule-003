#ifndef __DRV_UART_H__
#define __DRV_UART_H__
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
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

bool uart1_tx(uint8_t *buffer,size_t buffer_size,size_t timeout);
bool uart2_tx(uint8_t *buffer,size_t buffer_size,size_t timeout);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif
