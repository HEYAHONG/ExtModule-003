#include "main.h"
#include "hbox.h"
#include "product_config.h"
#include "drv_led.h"


static void xl2400p_reset(void)
{
    //��λ
    hsoftspi_xl2400p_write_register(XL2400P_W_REGISTER | XL2400P_CFG_TOP,0x1A);
    while(0xFE != hsoftspi_xl2400p_read_register(XL2400P_R_REGISTER | XL2400P_CFG_TOP))
    {
        HAL_Delay(5);
        hsoftspi_xl2400p_write_register(XL2400P_W_REGISTER | XL2400P_CFG_TOP,0xFE);
    }

    {
        /*
         * ���õ�ַRXADDR_P0/TXADDR
         * ������ַ
         */
        uint8_t address[5]= {0};
        product_config_public_channel_addr(address,sizeof(address));
        hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER | XL2400P_RX_ADDR_P0,address,sizeof(address));
        hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER | XL2400P_TX_ADDR,address,sizeof(address));
    }

    {
        /*
         * ���õ�ַP1~P5
         * ˽�е�ַ
         */
        uint8_t address[5]= {0};
        product_config_private_channel_addr(address,sizeof(address),(const uint8_t *)UID_BASE,16);
        hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER | XL2400P_RX_ADDR_P1,address,sizeof(address));
    }

    {
        /*
         * ����ģ��
         */
        uint8_t analog_cfg[6]= {0};
        hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER | XL2400P_ANALOG_CFG3,analog_cfg,sizeof(analog_cfg));
        analog_cfg[5]=((analog_cfg[5]&0xFF) | 0x6D);
        hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER | XL2400P_ANALOG_CFG3,analog_cfg,sizeof(analog_cfg));
    }

    {
        /*
         * ����FEATURE
         */
        uint8_t feature=hsoftspi_xl2400p_read_register(XL2400P_R_REGISTER| XL2400P_FEATURE);
        //��̬FEC+�׻�+��̬��Ч����+��̬ACK
        feature |= 0x1D;
        hsoftspi_xl2400p_write_register(XL2400P_W_REGISTER| XL2400P_FEATURE,feature);
    }

    {
        /*
         * ����ENAA
         */
        uint8_t enaa=hsoftspi_xl2400p_read_register(XL2400P_R_REGISTER| XL2400P_EN_AA);
        enaa |= 0x3F;
        hsoftspi_xl2400p_write_register(XL2400P_W_REGISTER| XL2400P_EN_AA,enaa);
    }
    {
        /*
         * ����DYNPD
         */
        uint8_t dynpd=hsoftspi_xl2400p_read_register(XL2400P_R_REGISTER| XL2400P_DYNPD);
        dynpd |= 0x3F;
        hsoftspi_xl2400p_write_register(XL2400P_W_REGISTER| XL2400P_DYNPD,dynpd);
    }
    {
        /*
        * ����PIPE
        */
        uint8_t rxaddr=hsoftspi_xl2400p_read_register(XL2400P_R_REGISTER| XL2400P_EN_RXADDR);
        rxaddr |= 0x3F;
        hsoftspi_xl2400p_write_register(XL2400P_W_REGISTER| XL2400P_EN_RXADDR,rxaddr);
    }
    {
        /*
        * �������Դ���
        */
        uint8_t retr=hsoftspi_xl2400p_read_register(XL2400P_R_REGISTER| XL2400P_SETUP_RETR);
        retr |= 0x3F;
        hsoftspi_xl2400p_write_register(XL2400P_W_REGISTER| XL2400P_SETUP_RETR,retr);
    }

    {
        /*
        * ����RF_SETUP
        */
        uint8_t rf_setup[2]= {0x02,0x10};
        hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER| XL2400P_RF_SETUP,rf_setup,sizeof(rf_setup));
    }

    {
        /*
        * ��������CFG_TOP
        */
        uint8_t cfg_top[2]= {0xFE,0x80};
        hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER| XL2400P_CFG_TOP,cfg_top,sizeof(cfg_top));
    }

}

static void xl2400p_defaults_state()
{
    {
        //�����ϵ�����֡
        xl2400p_set_channel(XL2400P_DEFAULT_RF_CHANNEL);
        xl2400p_set_is_prx(false);
        xl2400p_set_soft_ce(true);
        {
            //�ϵ���Ϣ(4�ֽ�ͷ(Ĭ������Ϊ0��������������)+16�ֽڲ�ƷID(UUID)+12�ֽ��Զ�������(����))
            uint8_t data[32]= {0};
            {
                //��дͷ(����Ϊ0���������ݰ�����)
                memset(data,0,4);
            }
            {
                //����ƷID
                size_t product_id_len=0;
                memcpy(&data[4],product_config_product_id_get(&product_id_len),product_id_len);
            }
            {
                //��д�Զ�������,ͨ������ʶ��(�û������ж���)
                /*
                 * Ĭ���Զ�������:hello��6�ֽڣ�+˽�е�ַ(6�ֽ�,����6�ֽ���Ҫ���)
                 */
                strcpy((char *)&data[20],"hello");
                product_config_private_channel_addr(&data[26],6,(const uint8_t *)UID_BASE,16);

                /*
                 * ���Զ������ݽ���chacha20����
                 */
                uint32_t counter=data[0]*(1<<0)+data[1]*(1<<8)+data[2]*(1<<16)+data[3]*(1<<24);
                product_config_data_chacha20_crypto(&data[20],12,counter,NULL,0);
            }
            xl2400p_write_tx_fifo_noack(data,sizeof(data));
        }
        HAL_Delay(1);

        //�������ͺ�ֹͣ����
        xl2400p_set_soft_ce(false);
        HAL_Delay(5);

        //��ջ���
        xl2400p_flush_rx();
        xl2400p_flush_tx();
        xl2400p_set_rf_status(0x70);
    }
    {
        //����״̬
        xl2400p_set_channel(XL2400P_DEFAULT_RF_CHANNEL-1);
        xl2400p_set_is_prx(true);
        xl2400p_set_soft_ce(true);
    }

}

/*
 * XL2400P�ļĴ����Ƿ�仯
 */
static bool xl2400p_register_table_dirty=true;
static uint8_t xl2400p_register_cfg_top[3]= {0};
static uint8_t xl2400p_register_en_aa[6]= {0};
static uint8_t xl2400p_register_en_rxaddr[1]= {0};
static uint8_t xl2400p_register_setup_aw[1]= {0};
static uint8_t xl2400p_register_setup_retr[4]= {0};
static uint8_t xl2400p_register_rf_ch[2]= {0};
static uint8_t xl2400p_register_setup_rf[3]= {0};
static uint8_t xl2400p_register_rx_addr_p0[5]= {0};
static uint8_t xl2400p_register_rx_addr_p1[5]= {0};
static uint8_t xl2400p_register_rx_addr_p2top5[4]= {0};
static uint8_t xl2400p_register_tx_addr[5]= {0};
static uint8_t xl2400p_register_dynpd[1]= {0};
static uint8_t xl2400p_register_feature[1]= {0};
/*
 * ����ĳЩ���üĴ���(���ַ�Ĵ�����,������������ĸı���Ҫ֪ͨӦ�ô���)
 * �û���Ҫ������Ӵ����ļĴ���
 */
static const struct
{
    uint8_t *reg_buffer;
    uint8_t  reg_address;
    uint8_t  reg_size;
}
xl2400p_register_table[]=
{
    {xl2400p_register_cfg_top,XL2400P_CFG_TOP,sizeof(xl2400p_register_cfg_top)},
    {xl2400p_register_en_aa,XL2400P_EN_AA,sizeof(xl2400p_register_en_aa)},
    {xl2400p_register_en_rxaddr,XL2400P_EN_RXADDR,sizeof(xl2400p_register_en_rxaddr)},
    {xl2400p_register_setup_aw,XL2400P_SETUP_AW,sizeof(xl2400p_register_setup_aw)},
    {xl2400p_register_setup_retr,XL2400P_SETUP_RETR,sizeof(xl2400p_register_setup_retr)},
    {xl2400p_register_rf_ch,XL2400P_RF_CH,sizeof(xl2400p_register_rf_ch)},
    {xl2400p_register_setup_rf,XL2400P_RF_SETUP,sizeof(xl2400p_register_setup_rf)},
    {xl2400p_register_rx_addr_p0,XL2400P_RX_ADDR_P0,sizeof(xl2400p_register_rx_addr_p0)},
    {xl2400p_register_rx_addr_p1,XL2400P_RX_ADDR_P1,sizeof(xl2400p_register_rx_addr_p1)},
    {xl2400p_register_rx_addr_p2top5,XL2400P_RX_ADDR_P2TOP5,sizeof(xl2400p_register_rx_addr_p2top5)},
    {xl2400p_register_tx_addr,XL2400P_TX_ADDR,sizeof(xl2400p_register_tx_addr)},
    {xl2400p_register_dynpd,XL2400P_DYNPD,sizeof(xl2400p_register_dynpd)},
    {xl2400p_register_feature,XL2400P_FEATURE,sizeof(xl2400p_register_feature)},
};
static void hsoftspi_xl2400p_register_check(void)
{
    if(xl2400p_register_table_dirty)
    {
        xl2400p_register_table_dirty=false;
        for(size_t i=0; i< sizeof(xl2400p_register_table)/sizeof(xl2400p_register_table[0]); i++)
        {
            hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER | xl2400p_register_table[i].reg_address,xl2400p_register_table[i].reg_buffer,  xl2400p_register_table[i].reg_size);
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
            hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER | xl2400p_register_table[i].reg_address,buffer,    xl2400p_register_table[i].reg_size);
            if(memcmp(buffer,xl2400p_register_table[i].reg_buffer,xl2400p_register_table[i].reg_size)!=0)
            {
                check_ok=false;
                /*
                * ����ĳЩ�Ĵ������Կ�����Ҫ��������
                */
                switch(xl2400p_register_table[i].reg_address)
                {
                case XL2400P_CFG_TOP:
                {
                    if((((buffer[0] ^ xl2400p_register_table[i].reg_buffer[0]) & 0x7F )==0) && (((buffer[1] ^ xl2400p_register_table[i].reg_buffer[1]) & 0xC0 )==0))
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
                hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER | xl2400p_register_table[i].reg_address,xl2400p_register_table[i].reg_buffer, xl2400p_register_table[i].reg_size);
            }

            //���Ϊ�ٷ�ֹ�ٴζ�ȡ
            xl2400p_register_table_dirty=false;
        }
    }
}

static xl2400p_loop_event_handler_t xl2400p_loop_event_handler=NULL;
void xl2400p_loop_set_event_handler(xl2400p_loop_event_handler_t evt_handler)
{
    xl2400p_loop_event_handler=evt_handler;
}
static xl2400p_loop_data_handler_t xl2400p_loop_data_handler=NULL;
void xl2400p_loop_set_data_handler(xl2400p_loop_data_handler_t data_handler)
{
    xl2400p_loop_data_handler=data_handler;
}
static void xl2400p_loop(void)
{
    if(xl2400p_get_soft_ce())
    {
        //����CE����
        uint8_t status=xl2400p_get_rf_status();
        if(xl2400p_get_is_prx())
        {
            //����ģʽ
            if((status&(1<< XL2400P_LOOP_EVENT_RX_DS))!=0)
            {
                //�����ݽ���FIFO
                led_trigger_net_rx();
                uint8_t px=((status >> 1)&0x7); //PIPE���
                uint8_t datalen=xl2400p_read_rx_fifo_length();
                if((hsoftspi_xl2400p_read_register(XL2400P_R_REGISTER| XL2400P_FEATURE)&(1 << 5))!=0)
                {
                    //128�ֽڰ�
                    uint8_t data[128]= {0};
                    xl2400p_read_rx_fifo(data,datalen);
                    if(xl2400p_loop_data_handler!=NULL)
                    {
                        xl2400p_loop_data_handler(data,datalen,px);
                    }
                }
                else
                {
                    //32�ֽڰ�
                    uint8_t data[32]= {0};
                    xl2400p_read_rx_fifo(data,datalen);
                    if(xl2400p_loop_data_handler!=NULL)
                    {
                        xl2400p_loop_data_handler(data,datalen,px);
                    }
                }
                if(xl2400p_loop_event_handler!=NULL)
                {
                    xl2400p_loop_event_handler(XL2400P_LOOP_EVENT_RX_DS);
                }
            }
        }
        else
        {
            //����ģʽ
            if(((status&(1<< XL2400P_LOOP_EVENT_TX_DS))!=0))
            {
                //�������
                led_trigger_net_tx();
                if(xl2400p_loop_event_handler!=NULL)
                {
                    xl2400p_loop_event_handler(XL2400P_LOOP_EVENT_TX_DS);
                }
            }

            if(((status&(1<< XL2400P_LOOP_EVENT_MAX_RT))!=0))
            {
                //����������Դ���
                if(xl2400p_loop_event_handler!=NULL)
                {
                    xl2400p_loop_event_handler(XL2400P_LOOP_EVENT_MAX_RT);
                }
                else
                {
                    /*
                     * ��������tx_fifo,оƬ��һֱ���Է���,���û�������¼�ʱ��Ҫע���ظ���������⡣
                     */
                    xl2400p_flush_tx();
                }
            }
        }

        //�����Ӧ��״̬λ
        xl2400p_set_rf_status(status);
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
    /*ʹ��ģ��ʱ��*/
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
}



#ifdef HRUNTIME_USING_INIT_SECTION
void  hsoftspi_xl2400p_init(const hruntime_function_t *func)
{
    hsoftspi_xl2400p_lowlevel_init();

    xl2400p_defaults_state();
}
HRUNTIME_INIT_EXPORT(softspi_xl2400p,0,hsoftspi_xl2400p_init,NULL);
#endif
#ifdef HRUNTIME_USING_LOOP_SECTION
void  hsoftspi_xl2400p_loop(const hruntime_function_t *func)
{
    //���Ĵ��������Ĵ�������ı�ʱ��λ���ָ���Ӧ�Ĵ���
    hsoftspi_xl2400p_register_check();

    //XL2400Pѭ��
    xl2400p_loop();
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
    CSN_Low();//����Ƭѡ
    hsoftspi_xl2400p_write_byte(RF_Reg);//д���ַ
    hsoftspi_xl2400p_write_byte(W_Data);//д������
    CSN_High();//д���������Ƭѡ
    if(RF_Reg >= XL2400P_W_REGISTER && RF_Reg < XL2400P_W_REGISTER*2)
    {
        //��ǼĴ������޸�
        xl2400p_register_table_dirty=true;
    }
}


uint8_t hsoftspi_xl2400p_read_register(uint8_t RF_Reg)
{
    uint8_t rTemp =0;
    CSN_Low();
    hsoftspi_xl2400p_write_byte(RF_Reg);//д���ַ+��ȡָ��
    rTemp = hsoftspi_xl2400p_read_byte();//��ȡ����
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
        //��ǼĴ������޸�
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

void xl2400p_reset_chip(void)
{
    xl2400p_reset();
}

void xl2400p_set_channel(uint16_t channel)
{
    if(channel < 2400)
    {
        channel = 2400;
    }
    if(channel > 2483)
    {
        channel = 2483;
    }
    uint8_t enaa=hsoftspi_xl2400p_read_register(XL2400P_R_REGISTER| XL2400P_EN_AA);
    enaa &= (~(0x40));
    hsoftspi_xl2400p_write_register(XL2400P_W_REGISTER| XL2400P_EN_AA,enaa);
    uint8_t buffer[2]= {0};
    buffer[0]=(uint8_t)channel;
    buffer[1]=(uint8_t)(channel >> 8);
    hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER+XL2400P_RF_CH,buffer,sizeof(buffer));
    enaa |= (0x40);
    hsoftspi_xl2400p_write_register(XL2400P_W_REGISTER| XL2400P_EN_AA,enaa);
}

bool xl2400p_get_soft_ce(void)
{
    uint8_t cfg_top=hsoftspi_xl2400p_read_register(XL2400P_R_REGISTER+XL2400P_CFG_TOP);
    return (cfg_top&0x01)!=0;
}

void xl2400p_set_soft_ce(bool ce_enable)
{
    uint8_t cfg_top=hsoftspi_xl2400p_read_register(XL2400P_R_REGISTER+XL2400P_CFG_TOP);
    if(ce_enable)
    {
        cfg_top|=0x01;
    }
    else
    {
        cfg_top&=(~0x01);
    }
    hsoftspi_xl2400p_write_register(XL2400P_W_REGISTER+XL2400P_CFG_TOP,cfg_top);
}

bool xl2400p_get_is_prx(void)
{
    uint8_t cfg_top[2]= {0};
    hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER+XL2400P_CFG_TOP,cfg_top,sizeof(cfg_top));
    return (cfg_top[1]&(1<<6))!=0;
}

void xl2400p_set_is_prx(bool prx_mode)
{
    uint8_t cfg_top[2]= {0};
    hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER+XL2400P_CFG_TOP,cfg_top,sizeof(cfg_top));
    if(prx_mode)
    {
        cfg_top[1]|= (1<<6);
    }
    else
    {
        cfg_top[1]&= (~(1<<6));
    }
    hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER+XL2400P_CFG_TOP,cfg_top,sizeof(cfg_top));
}

uint8_t xl2400p_get_rf_status(void)
{
    return hsoftspi_xl2400p_read_register(XL2400P_R_REGISTER+XL2400P_RF_STATUS);
}

void xl2400p_set_rf_status(uint8_t rf_status)
{
    hsoftspi_xl2400p_write_register(XL2400P_W_REGISTER+XL2400P_RF_STATUS,rf_status);
}

uint32_t xl2400p_get_rf_fifo_status(void)
{
    uint8_t fifo_status[4]= {0};
    hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER+XL2400P_STATUS_FIFO,fifo_status,sizeof(fifo_status));
    return fifo_status[0]*(1UL << 0) + fifo_status[1]*(1UL << 8) + fifo_status[2]*(1UL << 16) + fifo_status[3]*(1UL << 24);
}

void xl2400p_flush_tx(void)
{
    hsoftspi_xl2400p_write_register(XL2400P_FLUSH_TX,XL2400P_CMD_NOP);
}

void xl2400p_flush_rx(void)
{
    hsoftspi_xl2400p_write_register(XL2400P_FLUSH_RX,XL2400P_CMD_NOP);
}

void xl2400p_set_tx_addr(const void *addr,size_t addr_len)
{
    if(addr!=NULL)
    {
        if(addr_len > 5)
        {
            addr_len=5;
        }
        hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER| XL2400P_TX_ADDR,(uint8_t *)addr,addr_len);
    }
}

void xl2400p_get_tx_addr(void *addr,size_t addr_len)
{
    if(addr!=NULL)
    {
        if(addr_len > 5)
        {
            addr_len=5;
        }
        hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER| XL2400P_TX_ADDR,(uint8_t *)addr,addr_len);
    }
}

void xl2400p_set_rx_addr0(const void *addr,size_t addr_len)
{
    if(addr!=NULL)
    {
        if(addr_len > 5)
        {
            addr_len=5;
        }
        hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER| XL2400P_RX_ADDR_P0,(uint8_t *)addr,addr_len);
    }
}

void xl2400p_get_rx_addr0(void *addr,size_t addr_len)
{
    if(addr!=NULL)
    {
        if(addr_len > 5)
        {
            addr_len=5;
        }
        hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER| XL2400P_RX_ADDR_P0,(uint8_t *)addr,addr_len);
    }
}

void xl2400p_set_rx_addr1(const void *addr,size_t addr_len)
{
    if(addr!=NULL)
    {
        if(addr_len > 5)
        {
            addr_len=5;
        }
        hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER| XL2400P_RX_ADDR_P1,(uint8_t *)addr,addr_len);
    }
}

void xl2400p_get_rx_addr1(void *addr,size_t addr_len)
{
    if(addr!=NULL)
    {
        if(addr_len > 5)
        {
            addr_len=5;
        }
        hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER| XL2400P_RX_ADDR_P1,(uint8_t *)addr,addr_len);
    }
}

void xl2400p_set_rx_addr2top5(const void *addr,size_t addr_len)
{
    if(addr!=NULL)
    {
        if(addr_len > 4)
        {
            addr_len=4;
        }
        hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER| XL2400P_RX_ADDR_P2TOP5,(uint8_t *)addr,addr_len);
    }
}

void xl2400p_get_rx_addr2top5(void *addr,size_t addr_len)
{
    if(addr!=NULL)
    {
        if(addr_len > 4)
        {
            addr_len=4;
        }
        hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER| XL2400P_RX_ADDR_P2TOP5,(uint8_t *)addr,addr_len);
    }
}

void xl2400p_get_rx_addr(uint8_t px,void *addr,size_t addr_len)
{
    if(addr!=NULL && addr_len!=0)
    {
        switch(px)
        {
        case 0:
        {
            xl2400p_get_rx_addr0(addr,addr_len);
        }
        break;
        case 1:
        {
            xl2400p_get_rx_addr1(addr,addr_len);
        }
        break;
        case 2:
        case 3:
        case 4:
        case 5:
        {
            xl2400p_get_rx_addr1(addr,addr_len);
            uint8_t p2top5[4]= {0};
            xl2400p_get_rx_addr2top5(p2top5,sizeof(p2top5));
            ((uint8_t *)addr)[0]=p2top5[px-2];
        }
        break;
        default:
            break;
        }
    }
}

void xl2400p_set_rx_addr(uint8_t px,const void *addr,size_t addr_len)
{
    if(addr!=NULL && addr_len!=0)
    {
        switch(px)
        {
        case 0:
        {
            xl2400p_set_rx_addr0(addr,addr_len);
        }
        break;
        case 1:
        {
            xl2400p_set_rx_addr1(addr,addr_len);
        }
        break;
        case 2:
        case 3:
        case 4:
        case 5:
        {
            /*
            * ͨ��2~ͨ��5ֻ����������ֽڵ�ַ
            */
            uint8_t p2top5[4]= {0};
            xl2400p_get_rx_addr2top5(p2top5,sizeof(p2top5));
            p2top5[px-2]=((uint8_t *)addr)[0];
            xl2400p_set_rx_addr2top5(p2top5,sizeof(p2top5));
        }
        break;
        default:
            break;
        }
    }
}

uint8_t xl2400p_get_rx_payload_length(uint8_t px)
{
    uint8_t rx_pw[6]= {0};
    if(px >= 5)
    {
        px=5;
    }
    hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER| XL2400P_RX_PW_PX,rx_pw,sizeof(rx_pw));
    return rx_pw[px];
}

void xl2400p_set_rx_payload_length(uint8_t px,uint8_t pipe_width)
{
    uint8_t rx_pw[6]= {0};
    if(px >= 5)
    {
        px=5;
    }
    hsoftspi_xl2400p_read_register_buffer(XL2400P_R_REGISTER| XL2400P_RX_PW_PX,rx_pw,sizeof(rx_pw));
    rx_pw[px]=pipe_width;
    hsoftspi_xl2400p_write_register_buffer(XL2400P_W_REGISTER| XL2400P_RX_PW_PX,rx_pw,sizeof(rx_pw));
}

int8_t xl2400p_get_rx_pipe_num(void)
{
    int8_t ret=-1;
    uint8_t status=xl2400p_get_rf_status();
    if((status & (1<<6))!=0)
    {
        ret=((status >> 1)&0x07);
    }
    return ret;
}

void xl2400p_write_tx_fifo(const void *data,size_t data_len)
{
    if(data!=NULL && data_len > 0)
    {
        hsoftspi_xl2400p_write_register_buffer(XL2400P_W_TX_PLOAD,(uint8_t *)data,data_len);
    }
}

void xl2400p_write_tx_fifo_noack(const void *data,size_t data_len)
{
    if(data!=NULL && data_len > 0)
    {
        hsoftspi_xl2400p_write_register_buffer(XL2400P_W_TX_PLOAD_NOACK,(uint8_t *)data,data_len);
    }
}

void xl2400p_read_rx_fifo(void *data,size_t data_len)
{
    if(data!=NULL && data_len > 0)
    {
        hsoftspi_xl2400p_read_register_buffer(XL2400P_R_RX_PLOAD,(uint8_t *)data,data_len);
    }
}

uint8_t xl2400p_read_rx_fifo_length(void)
{
    return hsoftspi_xl2400p_read_register(XL2400P_R_RX_PL_WID);
}
