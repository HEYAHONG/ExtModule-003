#include "service_wireless.h"
#include "drv_softspi_xl2400p.h"

#ifndef SERVICE_WIRELESS_PRIORITY_LEVEL
#define SERVICE_WIRELESS_PRIORITY_LEVEL    31
#endif

/*
 * 发送时的序号
 */
static uint16_t wireless_frame_seq=0;

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
            wireless_frame_seq++;
            console_printf("wireless: event TX_DS");
        }
        break;
        case XL2400P_LOOP_EVENT_MAX_RT:
        {
            wireless_frame_seq++;
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
                //不是公共通道地址，可能是正在发送其它通道的数据将通道0作为了ACK接收通道。
                break;
            }
        }
        service_wireless_frame_header_t header= {0};
        memcpy(&header,data,sizeof(header));
        if(datalen==32 && header.flag.byte==0 && header.crc8==0 && header.seq==0)
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
        else if((header.flag.byte!=0 || header.crc8!=0 || header.seq!=0) && datalen > sizeof(header))
        {
            /*
             * 处理广播的其它数据
             */
            if(header.flag.b_crypt!=0)
            {
                //公共通道解密
                uint32_t packet_header=0;
                memcpy(&packet_header,data,sizeof(packet_header));
                product_config_data_chacha20_crypto(&data[sizeof(header)],datalen-sizeof(header),packet_header,NULL,0);
            }
            uint8_t buffer[128]= {0};
            size_t  buffer_len=datalen-sizeof(header);
            if(header.flag.b_compress!=0)
            {
                //对数据进行解压缩
                buffer_len=product_config_data_uncompress(buffer,sizeof(buffer),&data[sizeof(header)],buffer_len);
            }
            else
            {
                //直接复制
                memcpy(buffer,&data[sizeof(header)],buffer_len);
            }
            if(buffer_len == 0)
            {
                //解压缩出错
                break;
            }
            if(!hcrc_crc8_fast_check(buffer,buffer_len,header.crc8))
            {
                //CRC8校验错误
                break;
            }
            console_printf("wireless:boardcast frame!");
        }
    }
    break;
    default:
    {

    }
    break;
    }
}

size_t  service_wireless_frame_boardcast_generate(service_wireless_frame_t *frame,uint8_t * data,size_t datalen)
{
    size_t ret=0;
    if(frame==NULL || data == NULL || datalen ==0 || datalen > sizeof(frame->data))
    {
        return ret;
    }
    memset(frame,0,sizeof(service_wireless_frame_t));
    frame->header.flag.px=0;         //广播时通道必须设置为0
    frame->header.flag.type=0;       //广播时类型设置为0
    frame->header.crc8=hcrc_crc8_fast_calculate(data,datalen);
    frame->header.flag.b_compress=((0!=(ret=product_config_data_try_compress(frame->data,sizeof(frame->data),data,datalen)))?1:0);
    if(ret==0)
    {
        ret=datalen;
    }
    frame->header.seq=wireless_frame_seq;
    //默认都进行加密
    frame->header.flag.b_crypt=1;
    if(frame->header.flag.b_crypt!=0)
    {
        //进行公共通道的加密
        uint32_t packet_header=0;
        memcpy(&packet_header,&frame->header,sizeof(packet_header));
        product_config_data_chacha20_crypto(frame->data,ret,packet_header,NULL,0);
    }
    return ret+sizeof(frame->header);
};

#ifdef HRUNTIME_USING_INIT_SECTION
static void  hwireless_init(const hruntime_function_t *func)
{
    xl2400p_loop_set_event_handler(xl2400p_loop_event_handler);
    xl2400p_loop_set_data_handler(xl2400p_loop_data_handler);
    console_printf("service_wireless init!");
    //console_printf("wireless:sizeof(service_wireless_frame_header_t)=%d,sizeof(service_wireless_frame_t)=%d",(int)sizeof(service_wireless_frame_header_t),(int)sizeof(service_wireless_frame_t));
    hgetrandom(&wireless_frame_seq,sizeof(wireless_frame_seq),0);
    console_printf("wireless:wireless_frame_seq=%d",(int)wireless_frame_seq);
}
HRUNTIME_INIT_EXPORT(wireless,SERVICE_WIRELESS_PRIORITY_LEVEL,hwireless_init,NULL);
#endif
#ifdef HRUNTIME_USING_LOOP_SECTION
static void  hwireless_loop(const hruntime_function_t *func)
{

}
HRUNTIME_LOOP_EXPORT(wireless,SERVICE_WIRELESS_PRIORITY_LEVEL,hwireless_loop,NULL);
#endif

