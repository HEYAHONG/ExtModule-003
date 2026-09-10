#include "main.h"
#include "openblt_port.h"
#include "drv_psram.h"
size_t console_buffer_size(void);
void     Rs232Init(void)
{
    while(console_buffer_size() > 0)
    {
        hgetchar();
    }
}

void     Rs232TransmitPacket(blt_int8u *data, blt_int8u len)
{
    if(data!=NULL && len !=0)
    {
        hputchar(len);
        for(size_t i=0; i<len; i++)
        {
            hputchar(data[i]);
        }
    }
}

size_t console_size=0;
hdefaults_tick_t console_tick=0;
blt_bool Rs232ReceivePacket(blt_int8u *data, blt_int8u *len)
{
    if(data!=NULL && len != NULL)
    {
        size_t current_console_size=console_buffer_size();
        if(console_size!=current_console_size)
        {
            console_size=current_console_size;
            console_tick=hdefaults_tick_get();
        }
        else if(current_console_size > 0)
        {
            hdefaults_tick_t current_tick=hdefaults_tick_get();
            if(current_tick - console_tick > 20)
            {
                size_t max_len=hgetchar();
                if(max_len > BOOT_COM_RX_MAX_DATA)
                {
                    max_len = BOOT_COM_RX_MAX_DATA;
                }
                (*len)=0;
                while(console_buffer_size() > 0)
                {
                    data[(*len)]=hgetchar();
                    (*len)++;
                    if((*len) == max_len)
                    {
                        break;
                    }
                }
                if((*len)==max_len)
                {
                    return BLT_TRUE;
                }
                else
                {
                    while(console_buffer_size() > 0)
                    {
                        hgetchar();
                    }
                }
            }
        }
    }
    return BLT_FALSE;
}

bool openblt_load_rom_ok=false;

void     NvmInit(void)
{
    openblt_load_rom_ok=false;
}


blt_bool NvmWrite(blt_addr addr, blt_int32u len, blt_int8u *data)
{
    addr &= 0x0FFFFFFF;
    if(addr > psram_size())
    {
        return BLT_FALSE;
    }
    if(addr+len > psram_size())
    {
        len=psram_size()-addr;
    }
    if(data!=NULL)
    {
        psram_write(addr,data,len);
    }
    return BLT_TRUE;
}

blt_bool NvmErase(blt_addr addr, blt_int32u len)
{
    (void)addr;
    (void)len;
    return BLT_TRUE;
}


blt_bool NvmVerifyChecksum(void)
{
    /*
     * 此函数返回FALSE时bootloader不会启动用户程序
     */
    return openblt_load_rom_ok?BLT_TRUE:BLT_FALSE;
}


blt_addr NvmGetUserProgBaseAddress(void)
{
    return 0;
}


blt_bool NvmDone(void)
{
    openblt_load_rom_ok=true;
    return BLT_TRUE;
}


