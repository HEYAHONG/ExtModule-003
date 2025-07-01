#ifndef __SERVICE_WIRELESS_H__
#define __SERVICE_WIRELESS_H__
#include "service_shell.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef union service_wireless_frame_header_flag service_wireless_frame_header_flag_t;
union service_wireless_frame_header_flag
{
    uint8_t byte;
    struct
    {
        uint8_t b_crypt:1;
        uint8_t b_compress:1;
        uint8_t px:3;
        uint8_t type:3;
    };
};

typedef struct service_wireless_frame_header service_wireless_frame_header_t;
struct service_wireless_frame_header
{
    service_wireless_frame_header_flag_t    flag;
    uint8_t                                 crc8;
    uint16_t                                seq;
};


typedef struct service_wireless_frame service_wireless_frame_t;
struct service_wireless_frame
{
    service_wireless_frame_header_t header;
    uint8_t data[32-sizeof(service_wireless_frame_header_t)];
};

/*
 * 生成广播包(数据长度不得超过service_wireless_frame_t中data字段的长度),返回整个包的长度
 */
size_t  service_wireless_frame_boardcast_generate(service_wireless_frame_t *frame,uint8_t * data,size_t datalen);
typedef void (*service_wireless_frame_boardcast_rx_handler_t)(uint8_t *data,size_t datalen);
void service_wireless_frame_boardcast_set_rx_handler(service_wireless_frame_boardcast_rx_handler_t _rx_handler);
size_t service_wireless_frame_boardcast_transmit(uint8_t *data,size_t datalen);

#ifdef __cplusplus
}
#endif // __cplusplus   
#endif

