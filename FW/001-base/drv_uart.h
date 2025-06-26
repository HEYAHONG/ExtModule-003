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

#ifdef __cplusplus
}
#endif // __cplusplus
#endif
