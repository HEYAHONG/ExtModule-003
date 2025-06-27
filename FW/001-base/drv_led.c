#include "main.h"
#include "drv_led.h"
#include "hbox.h"

#ifndef DRV_LED_PRIORITY_LEVEL
#define DRV_LED_PRIORITY_LEVEL    3
#endif

#define LED_SYS_GPIO GPIOB
#define LED_SYS_PIN  GPIO_PIN_1
#define LED_NET_GPIO GPIOB
#define LED_NET_PIN  GPIO_PIN_0
#define LED_NET_TX_GPIO GPIOA
#define LED_NET_TX_PIN  GPIO_PIN_7
#define LED_NET_RX_GPIO GPIOA
#define LED_NET_RX_PIN  GPIO_PIN_6


#ifdef HRUNTIME_USING_INIT_SECTION
void  led_init(const hruntime_function_t *func)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef   GPIO_InitStruct;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pin   = LED_SYS_PIN;
    HAL_GPIO_Init(LED_SYS_GPIO, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_SYS_GPIO, LED_SYS_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin   = LED_NET_PIN;
    HAL_GPIO_Init(LED_NET_GPIO, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_NET_GPIO, LED_NET_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin   = LED_NET_TX_PIN;
    HAL_GPIO_Init(LED_NET_TX_GPIO, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_NET_TX_GPIO, LED_NET_TX_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pin   = LED_NET_RX_PIN;
    HAL_GPIO_Init(LED_NET_RX_GPIO, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_NET_RX_GPIO, LED_NET_RX_PIN, GPIO_PIN_SET);

}
HRUNTIME_INIT_EXPORT(led,DRV_LED_PRIORITY_LEVEL,led_init,NULL);
#endif
#ifdef HRUNTIME_USING_LOOP_SECTION
static uint32_t sys_led_mask=0x55555555;
static uint32_t net_led_mask=0xAAAAAAAA;
static uint8_t  net_tx_trigger_count=1;
static uint8_t  net_rx_trigger_count=1;
uint32_t led_get_sys_mask(void)
{
    return sys_led_mask;
}
void led_set_sys_mask(uint32_t mask)
{
    sys_led_mask=mask;
}
uint32_t led_get_net_mask(void)
{
    return net_led_mask;
}
void led_set_net_mask(uint32_t mask)
{
    net_led_mask=mask;
}
void led_trigger_net_rx(void)
{
    net_rx_trigger_count++;
    if(net_rx_trigger_count > 5)
    {
        net_rx_trigger_count=5;
    }
}
void led_trigger_net_tx(void)
{
    net_tx_trigger_count++;
    if(net_tx_trigger_count > 5)
    {
        net_tx_trigger_count=5;
    }
}
void  led_loop(const hruntime_function_t *func)
{
    static hdefaults_tick_t last_tick=0;
    if(hdefaults_tick_get()-last_tick > 200)
    {
        last_tick=hdefaults_tick_get();
        static uint8_t count=0;
        if(((1UL << count)&sys_led_mask)!=0)
        {
            HAL_GPIO_WritePin(LED_SYS_GPIO, LED_SYS_PIN, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(LED_SYS_GPIO, LED_SYS_PIN, GPIO_PIN_RESET);
        }
        if(((1UL << count)&net_led_mask)!=0)
        {
            HAL_GPIO_WritePin(LED_NET_GPIO, LED_NET_PIN, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(LED_NET_GPIO, LED_NET_PIN, GPIO_PIN_RESET);
        }
        if(net_tx_trigger_count > 0)
        {
            net_tx_trigger_count--;
            HAL_GPIO_WritePin(LED_NET_TX_GPIO, LED_NET_TX_PIN, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(LED_NET_TX_GPIO, LED_NET_TX_PIN, GPIO_PIN_RESET);
        }
        if(net_rx_trigger_count > 0)
        {
            net_rx_trigger_count--;
            HAL_GPIO_WritePin(LED_NET_RX_GPIO, LED_NET_RX_PIN, GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(LED_NET_RX_GPIO, LED_NET_RX_PIN, GPIO_PIN_RESET);
        }
        count++;
        if(count >= 32)
        {
            count=0;
        }

    }
}
HRUNTIME_LOOP_EXPORT(led,DRV_LED_PRIORITY_LEVEL,led_loop,NULL);
#endif

