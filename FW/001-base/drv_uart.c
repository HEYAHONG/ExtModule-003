#include "drv_uart.h"
#include "main.h"
#include "hbox.h"

#define UART1_TX_GPIO GPIOA
#define UART1_TX_PIN  GPIO_PIN_2
#define UART1_RX_GPIO GPIOA
#define UART1_RX_PIN  GPIO_PIN_3
#define UART2_TX_GPIO GPIOA
#define UART2_TX_PIN  GPIO_PIN_4
#define UART2_RX_GPIO GPIOA
#define UART2_RX_PIN  GPIO_PIN_5

static UART_HandleTypeDef uart1_handle= {0};
static UART_HandleTypeDef uart2_handle= {0};
UART_HandleTypeDef * uart1_get_handle(void)
{
    return &uart1_handle;
}
UART_HandleTypeDef * uart2_get_handle(void)
{
    return &uart2_handle;
}


#ifdef HRUNTIME_USING_INIT_SECTION
void  uart_init(const hruntime_function_t *func)
{
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef   GPIO_InitStruct= {0};

        /*
         * PA2->U1TX
         * PA3->U1RX
         */
        GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull  = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate=GPIO_AF1_USART1; /*选择UART1功能*/
        GPIO_InitStruct.Pin   = UART1_TX_PIN;
        HAL_GPIO_Init(UART1_TX_GPIO, &GPIO_InitStruct);
        GPIO_InitStruct.Pin   = UART1_RX_PIN;
        HAL_GPIO_Init(UART1_RX_GPIO, &GPIO_InitStruct);


        /*
         * PA4->U2TX
         * PA5->U2RX
         */
        GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull  = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate=GPIO_AF9_USART2;/*选择UART2功能*/
        GPIO_InitStruct.Pin   = UART2_TX_PIN;
        HAL_GPIO_Init(UART2_TX_GPIO, &GPIO_InitStruct);
        GPIO_InitStruct.Pin   = UART2_RX_PIN;
        HAL_GPIO_Init(UART2_RX_GPIO, &GPIO_InitStruct);
    }
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        uart1_handle.Instance          = USART1;
        uart1_handle.Init.BaudRate     = 115200;
        uart1_handle.Init.WordLength   = UART_WORDLENGTH_8B;
        uart1_handle.Init.StopBits     = UART_STOPBITS_1;
        uart1_handle.Init.Parity       = UART_PARITY_NONE;
        uart1_handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
        uart1_handle.Init.Mode         = UART_MODE_TX_RX;
        uart1_handle.Init.OverSampling = UART_OVERSAMPLING_16;
        uart1_handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        HAL_UART_Init(&uart1_handle);
    }

    {
        __HAL_RCC_USART2_CLK_ENABLE();
        uart2_handle.Instance          = USART2;
        uart2_handle.Init.BaudRate     = 115200;
        uart2_handle.Init.WordLength   = UART_WORDLENGTH_8B;
        uart2_handle.Init.StopBits     = UART_STOPBITS_1;
        uart2_handle.Init.Parity       = UART_PARITY_NONE;
        uart2_handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
        uart2_handle.Init.Mode         = UART_MODE_TX_RX;
        uart2_handle.Init.OverSampling = UART_OVERSAMPLING_16;
        uart2_handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
        HAL_UART_Init(&uart2_handle);
    }
}
HRUNTIME_INIT_EXPORT(uart,0,uart_init,NULL);
#endif
#ifdef HRUNTIME_USING_LOOP_SECTION
void  uart_loop(const hruntime_function_t *func)
{
}
HRUNTIME_LOOP_EXPORT(uart,0,uart_loop,NULL);
#endif

