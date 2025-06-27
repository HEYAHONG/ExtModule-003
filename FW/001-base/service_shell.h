#ifndef __SERVICE_SHELL_H__
#define __SERVICE_SHELL_H__
#include "product_config.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#if (PRODUCT_CONSOLE_ENABLE) > 0
#define console_printf(fmt,...) hprintf("[%08X] " fmt "\r\n" ,hdefaults_tick_get(),##__VA_ARGS__)
#else
#define console_printf(fmt,...)
#endif

#ifdef __cplusplus
}
#endif // __cplusplus	
	
#endif
