#include "main.h"
#include "hbox.h"
#include "drv_psram.h"


#ifdef HDEFAULTS_TICK_GET
hdefaults_tick_t HDEFAULTS_TICK_GET()
{
    return HAL_GetTick();
}
#endif

static int lock_nested=0;
#ifdef HDEFAULTS_MUTEX_LOCK
void HDEFAULTS_MUTEX_LOCK()
{
    if(lock_nested==0)
    {
        __disable_irq();
    }
    lock_nested++;
}
#endif

#ifdef HDEFAULTS_MUTEX_UNLOCK
void HDEFAULTS_MUTEX_UNLOCK()
{
    lock_nested--;
    if(lock_nested==0)
    {
        __enable_irq();
    }
}
#endif

#ifdef HABORT
void HABORT()
{
    NVIC_SystemReset();
}
#endif

#ifdef HEXIT
void HEXIT(int exit_code)
{
    (void)exit_code;
    habort();
}
#endif

#ifdef HFWRITE
size_t HFWRITE(const void*buffer,size_t obj_size,size_t obj_count,FILE * stream )
{
    (void)buffer;
    (void)obj_size;
    (void)obj_count;
    (void)stream;
    return 0;
}
#endif

static size_t hbox_vm_default_read(const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,uint8_t *data,size_t length)
{
    (void)map_item;
    if(data==NULL)
    {
        return 0;
    }
    for(size_t i=0; i<length; i++)
    {
        data[i]=(*((__I uint8_t *)(address+i)));
    }
    return length;
};

static size_t hbox_vm_default_write(const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,const uint8_t *data,size_t length)
{
    (void)map_item;
    if(data==NULL)
    {
        return 0;
    }
    for(size_t i=0; i<length; i++)
    {
        (*((__O uint8_t *)(address+i)))=data[i];
    }
    return length;
};

static size_t hbox_vm_psram_read(const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,uint8_t *data,size_t length)
{
    if(data==NULL || map_item==0)
    {
        return 0;
    }
    if(address > map_item->map_address+psram_size())
    {
        return 0;
    }
    if((address+length-map_item->map_address)>psram_size())
    {
        length=map_item->map_address+psram_size()-address;
    }
    psram_read(address-map_item->map_address,data,length);
    return length;
};

static size_t hbox_vm_psram_write(const hsoftwarevirtualmemory_map_item_t *map_item,uintptr_t address,const uint8_t *data,size_t length)
{
    if(data==NULL || map_item==0)
    {
        return 0;
    }
    if(address > map_item->map_address+psram_size())
    {
        return 0;
    }
    if((address+length-map_item->map_address)>psram_size())
    {
        length=map_item->map_address+psram_size()-address;
    }
    psram_write(address-map_item->map_address,data,length);
    return length;
};


static const hsoftwarevirtualmemory_map_item_t hbox_vm_map_table[]=
{
    /*
     * 内部Flash地址
     */
    {0x08000000,0x00010000,hbox_vm_default_read,NULL,NULL},
    /*
     * 内部SRAM地址
     */
    {0x20000000,0x00002000,hbox_vm_default_read,hbox_vm_default_write,NULL},
    /*
     * 外部PSRAM(通过SPI连接)
     */
    {0x30000000,0x01000000,hbox_vm_psram_read,hbox_vm_psram_write,NULL},
    /*
     * 映射表结束
     */
    {0,0,NULL,NULL,NULL}
};
const struct hsoftwarevirtualmemory_map_item * const hbox_vm_map=hbox_vm_map_table;


