#include "service_wireless.h"
#include "drv_softspi_xl2400p.h"

#ifndef SERVICE_WIRELESS_PRIORITY_LEVEL
#define SERVICE_WIRELESS_PRIORITY_LEVEL    31
#endif

void xl2400p_loop_event_handler(xl2400p_loop_event_t evt)
{
    {
        //打印事件
        switch(evt)
        {
        case XL2400P_LOOP_EVENT_RX_DS:
        {
            console_printf("wireless: event RX_DS");
        }
        break;
        case XL2400P_LOOP_EVENT_TX_DS:
        {
            console_printf("wireless: event TX_DS");
        }
        break;
        case XL2400P_LOOP_EVENT_MAX_RT:
        {
            console_printf("wireless: event MAX_RT");
        }
        break;
        default:
        {
            console_printf("wireless: event UNKOWN");
        }
        break;
        }
    }
}

void xl2400p_loop_data_handler(uint8_t *data,size_t datalen,uint8_t px)
{
    if(data==NULL || datalen==0)
    {
        return;
    }
    switch(px)
    {
    case 0: /* 通道0,公共通道 */
    {
        {
            uint8_t public_addr[5]= {0};
            product_config_public_channel_addr(public_addr,sizeof(public_addr));
            uint8_t pipe_addr[sizeof(public_addr)]= {0};
            xl2400p_get_rx_addr(px,pipe_addr,sizeof(pipe_addr));
            if(memcmp(public_addr,pipe_addr,sizeof(pipe_addr))!=0)
            {
                //不是公共通道地址，可能是正在发送其它通道的数据将通道0作为了ACK。
                break;
            }
        }
        if(datalen==32)
        {
            /*
             * 处理上电数据帧
             */
            uint32_t packet_header=0;
            memcpy(&packet_header,data,sizeof(packet_header));
            if(packet_header==0 && memcmp(&data[sizeof(packet_header)],product_config_product_id_get(NULL),16)==0)
            {
                /*
                * 处理上电数据帧(解密加密部分)
                */
                product_config_data_chacha20_crypto(&data[20],12,packet_header,NULL,0);
                char buffer[64+1]= {0};
                hbase16_encode_with_null_terminator(buffer,sizeof(buffer),data,datalen);
                if(strcmp("hello",(const char *)&data[20])==0)
                {
                    /*
                     * 默认的hello数据帧
                     */
                    console_printf("wireless: powerup frame addr=%s",&buffer[26*2]);
                }
            }
        }
    }
    break;
    default:
    {

    }
    break;
    }
}

#ifdef HRUNTIME_USING_INIT_SECTION
static void  hwireless_init(const hruntime_function_t *func)
{
    xl2400p_loop_set_event_handler(xl2400p_loop_event_handler);
    xl2400p_loop_set_data_handler(xl2400p_loop_data_handler);
}
HRUNTIME_INIT_EXPORT(wireless,SERVICE_WIRELESS_PRIORITY_LEVEL,hwireless_init,NULL);
#endif
#ifdef HRUNTIME_USING_LOOP_SECTION
static void  hwireless_loop(const hruntime_function_t *func)
{

}
HRUNTIME_LOOP_EXPORT(wireless,SERVICE_WIRELESS_PRIORITY_LEVEL,hwireless_loop,NULL);
#endif

