#ifndef __DRV_PSRAM_H__
#define __DRV_PSRAM_H__
#include "stdint.h"
#include "stdlib.h"
#include "main.h"
#include "hbox.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * 读取PSRAM ID
 */
uint64_t psram_readid(void);

/*
 * 读取数据
 */
void psram_read(uint32_t addr,uint8_t *buff,size_t buff_size);

/*
 * 写入数据
 */
void psram_write(uint32_t addr,const uint8_t *buff,size_t buff_size);

/*
 * 获取检测到的PSRAM大小
 */
size_t psram_size();

/*
 * PSRAM分区
 */
enum
{
    PSRAM_LINK_ADDRESS=  0x00000000,
    PSRAM_NET_ADDRESS=   0x00200000,
    PSRAM_USR_ADDRESS=   0x00400000,
    PSRAM_END_ADDRESS=   0x00800000,
};

/*
 * PSRAM分区分配
 */
extern const hmemorystaticallocator_item_t * const psram_alloc_table;


#ifdef __cplusplus
}
#endif // __cplusplus


#endif
