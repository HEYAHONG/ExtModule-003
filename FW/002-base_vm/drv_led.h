#ifndef __DRV_LED_H__
#define __DRV_LED_H__
#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

uint32_t led_get_sys_mask(void);
void led_set_sys_mask(uint32_t mask);

uint32_t led_get_net_mask(void);
void led_set_net_mask(uint32_t mask);

void led_trigger_net_rx(void);
void led_trigger_net_tx(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif

