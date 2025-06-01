#ifndef __PRODUCT_CONFIG_H__
#define __PRODUCT_CONFIG_H__
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "hbox.h"
#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*
 * 产品名称
 */
#ifdef  PRODUCT_NAME
#define PRODUCT_NAME ExtModule_003
#endif

/*
 * 产品密码
 */
#ifdef  PRODUCT_PSK
#define PRODUCT_NAME ExtModule_003_PSK
#endif

/*
 * 获取产品ID(通过产品名称计算得来),通常128位。
 * 可用于区分不同的网络。
 */
const uint8_t *	product_config_product_id_get(size_t *product_id_len);

/*
 * 获取产品密钥(通过产品ID与),通常256位。
 * 可用于通信过程中使用AES或者ChaCha20加密的密钥。
 */
const uint8_t *	product_config_product_key_get(size_t *product_key_len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
