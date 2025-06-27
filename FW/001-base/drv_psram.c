#include "drv_psram.h"
#include "main.h"
#include "hbox.h"

#ifndef DRV_PSRAM_PRIORITY_LEVEL
#define DRV_PSRAM_PRIORITY_LEVEL    2
#endif

#define PSRAM_CS_GPIO    GPIOA
#define PSRAM_CS_PIN     GPIO_PIN_15
#define PSRAM_SCK_GPIO   GPIOB
#define PSRAM_SCK_PIN    GPIO_PIN_3
#define PSRAM_MISO_GPIO  GPIOB
#define PSRAM_MISO_PIN   GPIO_PIN_4
#define PSRAM_MOSI_GPIO  GPIOB
#define PSRAM_MOSI_PIN   GPIO_PIN_5


#define CS_HIGH()    HAL_GPIO_WritePin(PSRAM_CS_GPIO, PSRAM_CS_PIN, GPIO_PIN_SET)
#define CS_LOW()     HAL_GPIO_WritePin(PSRAM_CS_GPIO, PSRAM_CS_PIN, GPIO_PIN_RESET)
#define SCK_HIGH()   HAL_GPIO_WritePin(PSRAM_SCK_GPIO, PSRAM_SCK_PIN, GPIO_PIN_SET)
#define SCK_LOW()    HAL_GPIO_WritePin(PSRAM_SCK_GPIO, PSRAM_SCK_PIN, GPIO_PIN_RESET)
#define MOSI_HIGH()  HAL_GPIO_WritePin(PSRAM_MOSI_GPIO, PSRAM_MOSI_PIN, GPIO_PIN_SET)
#define MOSI_LOW()   HAL_GPIO_WritePin(PSRAM_MOSI_GPIO, PSRAM_MOSI_PIN, GPIO_PIN_RESET)
#define MISO()       (HAL_GPIO_ReadPin(PSRAM_MISO_GPIO, PSRAM_MISO_PIN)!=GPIO_PIN_RESET)

static uint8_t spi_transport_byte(uint8_t byte)
{
    uint8_t ret=0;
    SCK_LOW();
    for(size_t i=0; i<8; i++)
    {
        SCK_LOW();
        ret <<= 1;
        if(MISO())
        {
            ret |= 0x01;
        }
        if((0x80&byte)!=0)
        {
            MOSI_HIGH();
        }
        else
        {
            MOSI_LOW();
        }
        byte <<= 1;
        SCK_HIGH();
    }
    SCK_LOW();
    return ret;
}

static void spi_transport_start()
{
    CS_LOW();
}

static void spi_transport_end()
{
    CS_HIGH();
}

static void spi_transport_bytes(const uint8_t *tx_buff,size_t tx_buff_size,uint8_t *rx_buff,size_t rx_buff_size)
{
    spi_transport_start();
    if(tx_buff!=NULL && tx_buff_size!=0)
    {
        for(size_t i=0; i<tx_buff_size; i++)
        {
            spi_transport_byte(tx_buff[i]);
        }
    }
    if(rx_buff!=NULL && rx_buff_size!=0)
    {
        for(size_t i=0; i<rx_buff_size; i++)
        {
            rx_buff[i]=spi_transport_byte(rx_buff[i]);
        }
    }
    spi_transport_end();
}

static void spi_transport_bytes2(const uint8_t *tx_buff,size_t tx_buff_size,const uint8_t *tx2_buff,size_t tx2_buff_size)
{
    spi_transport_start();
    if(tx_buff!=NULL && tx_buff_size!=0)
    {
        for(size_t i=0; i<tx_buff_size; i++)
        {
            spi_transport_byte(tx_buff[i]);
        }
    }
    if(tx2_buff!=NULL && tx2_buff_size!=0)
    {
        for(size_t i=0; i<tx2_buff_size; i++)
        {
            spi_transport_byte(tx2_buff[i]);
        }
    }
    spi_transport_end();
}

uint64_t psram_readid(void)
{
    uint64_t ret=0;
    const uint8_t cmd[4]= {0x9f,0x00,0x00,0x00};
    uint8_t data[8]= {0};
    spi_transport_bytes(cmd,sizeof(cmd),data,sizeof(data));
    ret=data[7]*(1ULL << 56)+data[6]*(1ULL << 48)+data[5]*(1ULL << 40)+data[4]*(1ULL << 32)+data[3]*(1ULL << 24)+data[2]*(1ULL << 16)+data[1]*(1ULL << 8)+data[0]*(1ULL << 0);
    return ret;
}

void psram_read(uint32_t addr,uint8_t *buff,size_t buff_size)
{
    const uint8_t cmd[4]= {0x03,(addr>>16)&0xFF,(addr>>8)&0xFF,(addr>>0)&0xFF};
    spi_transport_bytes(cmd,sizeof(cmd),buff,buff_size);
}

void psram_write(uint32_t addr,const uint8_t *buff,size_t buff_size)
{
    const uint8_t cmd[4]= {0x02,(addr>>16)&0xFF,(addr>>8)&0xFF,(addr>>0)&0xFF};
    spi_transport_bytes2(cmd,sizeof(cmd),buff,buff_size);
}

static  size_t psram_detect_size=0;
size_t psram_size()
{
    return psram_detect_size;
}

#ifdef HRUNTIME_USING_INIT_SECTION
void  psram_init(const hruntime_function_t *func)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef   GPIO_InitStruct= {0};
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin   = PSRAM_CS_PIN;
    HAL_GPIO_Init(PSRAM_CS_GPIO, &GPIO_InitStruct);
    HAL_GPIO_WritePin(PSRAM_CS_GPIO, PSRAM_CS_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin   = PSRAM_SCK_PIN;
    HAL_GPIO_Init(PSRAM_SCK_GPIO, &GPIO_InitStruct);
    HAL_GPIO_WritePin(PSRAM_SCK_GPIO, PSRAM_SCK_PIN, GPIO_PIN_RESET);
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin   = PSRAM_MOSI_PIN;
    HAL_GPIO_Init(PSRAM_MOSI_GPIO, &GPIO_InitStruct);
    HAL_GPIO_WritePin(PSRAM_MOSI_GPIO, PSRAM_MOSI_PIN, GPIO_PIN_SET);
    GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pin   = PSRAM_MISO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    HAL_GPIO_Init(PSRAM_MISO_GPIO, &GPIO_InitStruct);

    {
        /*
         * 检测PSRAM大小
         * 缓冲区,0=随机数,1=原值,2=读取值(用于校验是否成功读取)
         */
        uint32_t end_address=0x00080000;                            //基础大小512KB
        for(size_t i=0; i<5; i++)
        {
            uint8_t buffer[3][32]= {0};
            hgetrandom(buffer[0],sizeof(buffer[0]),0);
            psram_read(end_address-sizeof(buffer[1]),buffer[1],sizeof(buffer[1]));
            psram_write(end_address-sizeof(buffer[0]),buffer[0],sizeof(buffer[0]));
            memset(buffer[2],0,sizeof(buffer[2]));
            psram_read(end_address-sizeof(buffer[2]),buffer[2],sizeof(buffer[2]));
            psram_write(end_address-sizeof(buffer[1]),buffer[1],sizeof(buffer[1]));
            if(memcmp(buffer[0],buffer[2],sizeof(buffer[0]))==0)
            {
                psram_detect_size=end_address;
            }
            else
            {
                break;
            }
            end_address*=2;
        }
    }

}
HRUNTIME_INIT_EXPORT(psram,DRV_PSRAM_PRIORITY_LEVEL,psram_init,NULL);
#endif
#ifdef HRUNTIME_USING_LOOP_SECTION
void  psram_loop(const hruntime_function_t *func)
{
}
HRUNTIME_LOOP_EXPORT(psram,DRV_PSRAM_PRIORITY_LEVEL,psram_loop,NULL);
#endif

