#include "main.h"
#include "hbox.h"


static void xl2400p_reset(void)
{
    //复位
    uint8_t cfg_top=0;
    cfg_top=hsoftspi_xl2400p_read_register(XL2400P_R_REGISTER | XL2400P_CFG_TOP);
    hsoftspi_xl2400p_write_register(XL2400P_W_REGISTER | XL2400P_CFG_TOP,cfg_top & (~0x04) | 0x02);
    HAL_Delay(1);
    cfg_top=hsoftspi_xl2400p_read_register(XL2400P_R_REGISTER | XL2400P_CFG_TOP);
    hsoftspi_xl2400p_write_register(XL2400P_W_REGISTER | XL2400P_CFG_TOP,0xFE);
    HAL_Delay(1);
}

/*
 * XL2400P的寄存器是否变化
 */
static bool xl2400p_register_table_dirty=true;
static uint8_t xl2400p_register_cfg_top_8=0;
static uint8_t xl2400p_register_rx_addr_p1_40[5]= {0};
/*
 * 对于某些配置寄存器(如地址寄存器等,若发生了意外的改变需要通知应用处理)
 * 用户需要自行添加待检查的寄存器
 */
static struct
{
    uint8_t *reg_buffer;
    uint8_t  reg_address;
    uint8_t  reg_size;
}
xl2400p_register_table[]=
{
    {&xl2400p_register_cfg_top_8,XL2400P_CFG_TOP,sizeof(xl2400p_register_cfg_top_8)},
    {xl2400p_register_rx_addr_p1_40,XL2400P_RX_ADDR_P1,sizeof(xl2400p_register_rx_addr_p1_40)},
};
static void hsoftspi_xl2400p_register_check(void)
{
    if(xl2400p_register_table_dirty)
    {
        xl2400p_register_table_dirty=false;
        for(size_t i=0; i< sizeof(xl2400p_register_table)/sizeof(xl2400p_register_table[0]); i++)
        {
            hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER | xl2400p_register_table[i].reg_address,xl2400p_register_table[i].reg_buffer,	xl2400p_register_table[i].reg_size);
        }
    }
    else
    {
        bool check_ok=true;
        for(size_t i=0; i< sizeof(xl2400p_register_table)/sizeof(xl2400p_register_table[0]); i++)
        {
            uint8_t buffer[16]= {0};
            if(sizeof(buffer) < xl2400p_register_table[i].reg_size)
            {
                continue;
            }
            hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER | xl2400p_register_table[i].reg_address,buffer,	xl2400p_register_table[i].reg_size);
            if(memcmp(buffer,xl2400p_register_table[i].reg_buffer,xl2400p_register_table[i].reg_size)!=0)
            {
                check_ok=false;
                /*
                * 对于某些寄存器而言可能需要独立处理
                */
                switch(xl2400p_register_table[i].reg_address)
                {
                case XL2400P_CFG_TOP:
                {
                    if(((buffer[0] ^ xl2400p_register_table[i].reg_buffer[0]) & 0x3F )!=0)
                    {
                        check_ok=true;
                    }
                }
                break;
                default:
                {
                }
                break;
                }
                if(!check_ok)
                {
                    break;
                }
            }

        }

        if(!check_ok)
        {
            xl2400p_reset();

            for(size_t i=0; i< sizeof(xl2400p_register_table)/sizeof(xl2400p_register_table[0]); i++)
            {
                hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER | xl2400p_register_table[i].reg_address,xl2400p_register_table[i].reg_buffer,	xl2400p_register_table[i].reg_size);
            }

            //标记为假防止再次读取
            xl2400p_register_table_dirty=false;
        }
    }
}


#define CSN_Low()                       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
#define CSN_High()                      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);

#define SCK_High()                      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
#define SCK_Low()                       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);

#define SPI_MOSI_High()                 HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_SET);
#define SPI_MOSI_Low()                  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_3, GPIO_PIN_RESET);

#define Read_SPI_MISO()                 HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1)

static void hsoftspi_xl2400p_lowlevel_init()
{
    /*使能模块时钟*/
    __HAL_RCC_GPIOF_CLK_ENABLE();//PF
    __HAL_RCC_GPIOA_CLK_ENABLE();//PA

    GPIO_InitTypeDef   GPIO_InitStruct;

    GPIO_InitStruct.Pin   = GPIO_PIN_1;
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = GPIO_PIN_3;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    CSN_High();
    SCK_Low();


    xl2400p_reset();

    {
        /*
         * 配置地址
         * 将通道1~5地址的高32位使用CRC32通过芯片UID算出,用作唯一默认地址，通道0仍然使用默认地址
         */
        uint32_t uid_crc=hcrc_crc32_fast_calculate((const uint8_t *)UID_BASE,16);
        uint8_t address[5]= {0};
        hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER | XL2400P_RX_ADDR_P1,address,sizeof(address));
        memcpy(&address[sizeof(address)-sizeof(uid_crc)],&uid_crc,sizeof(uid_crc));
        hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER | XL2400P_RX_ADDR_P1,address,sizeof(address));
    }
}

#ifdef HRUNTIME_USING_INIT_SECTION
void  hsoftspi_xl2400p_init(const hruntime_function_t *func)
{
    hsoftspi_xl2400p_lowlevel_init();
}
HRUNTIME_INIT_EXPORT(softspi_xl2400p,0,hsoftspi_xl2400p_init,NULL);
#endif
#ifdef HRUNTIME_USING_LOOP_SECTION
void  hsoftspi_xl2400p_loop(const hruntime_function_t *func)
{
    //检查寄存器，当寄存器意外改变时复位并恢复相应寄存器
    hsoftspi_xl2400p_register_check();
}
HRUNTIME_LOOP_EXPORT(softspi_xl2400p,0,hsoftspi_xl2400p_loop,NULL);
#endif

static void hsoftspi_xl2400p_write_byte(uint8_t buff)
{
    uint8_t i = 0;

    for(i = 0; i < 8; i++)
    {
        SCK_Low();

        if(buff & 0x80)
        {
            SPI_MOSI_High();
        }
        else
        {
            SPI_MOSI_Low();
        }

        buff = buff << 1;

        SCK_High();
    }
    SPI_MOSI_High();
    SCK_Low();
}


static uint8_t hsoftspi_xl2400p_read_byte(void)
{

    uint8_t  buff = 0;
    uint8_t  i = 0;

    for(i = 0; i < 8; i++)
    {
        SCK_Low();
        buff = buff << 1;
        SCK_High();
        if(Read_SPI_MISO())
        {
            buff |= 0x01;
        }
    }
    SCK_Low();

    return buff;
}


void hsoftspi_xl2400p_write_register(uint8_t RF_Reg,uint8_t W_Data)
{
    CSN_Low();//拉低片选
    hsoftspi_xl2400p_write_byte(RF_Reg);//写入地址
    hsoftspi_xl2400p_write_byte(W_Data);//写入数据
    CSN_High();//写入完成拉高片选
    if(RF_Reg >= XL2400P_W_REGISTER && RF_Reg < XL2400P_W_REGISTER*2)
    {
        //标记寄存器已修改
        xl2400p_register_table_dirty=true;
    }
}


uint8_t hsoftspi_xl2400p_read_register(uint8_t RF_Reg)
{
    uint8_t rTemp =0;
    CSN_Low();
    hsoftspi_xl2400p_write_byte(RF_Reg);//写入地址+读取指令
    rTemp = hsoftspi_xl2400p_read_byte();//读取数据
    CSN_High();
    return rTemp;
}


void hsoftspi_xl2400p_write_register_buffer(uint8_t RF_Reg, uint8_t *pBuff, uint8_t Len)
{
    uint8_t rTemp = 0;
    uint8_t i = 0;
    CSN_Low();
    hsoftspi_xl2400p_write_byte(RF_Reg);
    for(i = 0 ; i < Len ; i++)
    {
        rTemp = pBuff[i];
        hsoftspi_xl2400p_write_byte(rTemp);
    }
    CSN_High();
    if(RF_Reg >= XL2400P_W_REGISTER && RF_Reg < XL2400P_W_REGISTER*2)
    {
        //标记寄存器已修改
        xl2400p_register_table_dirty=true;
    }
}


void hsoftspi_xl2400p_read_register_buffer(uint8_t RF_Reg, uint8_t *pBuff, uint8_t Len)
{
    uint8_t i=0;
    CSN_Low();
    hsoftspi_xl2400p_write_byte(RF_Reg);
    for(i = 0 ; i < Len ; i++)
    {
        pBuff[i] = hsoftspi_xl2400p_read_byte();
    }
    CSN_High();
}


