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

#ifndef DRV_UART_PRIORITY_LEVEL
#define DRV_UART_PRIORITY_LEVEL    1
#endif

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
static void  uart_init(const hruntime_function_t *func)
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
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
        HAL_NVIC_EnableIRQ(USART1_IRQn);

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
        HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
}
HRUNTIME_INIT_EXPORT(uart,DRV_UART_PRIORITY_LEVEL,uart_init,NULL);
bool uart1_tx(uint8_t *buffer,size_t buffer_size,size_t timeout)
{
    if(buffer!=NULL && buffer_size!=0)
    {
        return HAL_OK==HAL_UART_Transmit(&uart1_handle,(uint8_t *)buffer,buffer_size,timeout);
    }
    return false;
}
bool uart2_tx(uint8_t *buffer,size_t buffer_size,size_t timeout)
{
    if(buffer!=NULL && buffer_size!=0)
    {
        return HAL_OK==HAL_UART_Transmit(&uart2_handle,(uint8_t *)buffer,buffer_size,timeout);
    }
    return false;
}
#endif
#ifdef HRUNTIME_USING_LOOP_SECTION
static uint8_t uart1_rx_buffer[64]= {0};
static uint8_t uart2_rx_buffer[64]= {0};
static uart_rx_handler_t uart1_rx_handler=NULL;
static uart_rx_handler_t uart2_rx_handler=NULL;
void uart1_set_rx_handler(uart_rx_handler_t handler)
{
    uart1_rx_handler=handler;
}
void uart2_set_rx_handler(uart_rx_handler_t handler)
{
    uart2_rx_handler=handler;
}
static void  uart_loop(const hruntime_function_t *func)
{
    static hdefaults_tick_t last_tick=0;
    if(hdefaults_tick_get()-last_tick > 1)
    {
        last_tick=hdefaults_tick_get();
        {
            uint8_t state=HAL_UART_GetState(&uart1_handle);
            static uint8_t last_rx_size=0;
            uint8_t rx_size=uart1_handle.RxXferSize-uart1_handle.RxXferCount;
            if(rx_size > 0)
            {
                if(last_rx_size!=rx_size)
                {
                    last_rx_size=rx_size;
                }
                else
                {
                    //处理接收的字节
                    HAL_UART_AbortReceive_IT(&uart1_handle);
                    if(uart1_rx_handler!=NULL)
                    {
                        uart1_rx_handler(uart1_rx_buffer,rx_size);
                    }
                    else
                    {
                        uart1_tx(uart1_rx_buffer,rx_size,10+rx_size);
                    }
                }
            }
            if((state)==HAL_UART_STATE_READY)
            {
                HAL_UART_Receive_IT(&uart1_handle,uart1_rx_buffer,sizeof(uart1_rx_buffer));
            }
            if((state)==HAL_UART_STATE_TIMEOUT || (state)==HAL_UART_STATE_ERROR)
            {
                //运行过程中出现超时或者错误
                if(rx_size!=0 && uart1_rx_handler!=NULL)
                {
                    uart1_rx_handler(uart1_rx_buffer,rx_size);
                }
                else
                {
                    uart1_tx(uart1_rx_buffer,rx_size,10+rx_size);
                }
                HAL_UART_AbortReceive_IT(&uart1_handle);
                HAL_UART_Receive_IT(&uart1_handle,uart1_rx_buffer,sizeof(uart1_rx_buffer));
            }
        }
        {
            uint8_t state=HAL_UART_GetState(&uart2_handle);
            static uint8_t last_rx_size=0;
            uint8_t rx_size=uart2_handle.RxXferSize-uart2_handle.RxXferCount;
            if(rx_size > 0)
            {
                if(last_rx_size!=rx_size)
                {
                    last_rx_size=rx_size;
                }
                else
                {
                    //处理接收的字节
                    HAL_UART_AbortReceive_IT(&uart2_handle);
                    if(uart2_rx_handler!=NULL)
                    {
                        uart2_rx_handler(uart2_rx_buffer,rx_size);
                    }
                    else
                    {
                        uart2_tx(uart2_rx_buffer,rx_size,10+rx_size);
                    }
                }
            }
            if((state)==HAL_UART_STATE_READY)
            {
                HAL_UART_Receive_IT(&uart2_handle,uart2_rx_buffer,sizeof(uart2_rx_buffer));
            }
            if((state)==HAL_UART_STATE_TIMEOUT || (state)==HAL_UART_STATE_ERROR)
            {
                //运行过程中出现超时或者错误
                if(rx_size!=0 && uart2_rx_handler!=NULL)
                {
                    uart2_rx_handler(uart2_rx_buffer,rx_size);
                }
                else
                {
                    uart2_tx(uart2_rx_buffer,rx_size,10+rx_size);
                }
                HAL_UART_AbortReceive_IT(&uart2_handle);
                HAL_UART_Receive_IT(&uart2_handle,uart2_rx_buffer,sizeof(uart2_rx_buffer));
            }
        }
    }

}
HRUNTIME_LOOP_EXPORT(uart,DRV_UART_PRIORITY_LEVEL,uart_loop,NULL);
#endif

