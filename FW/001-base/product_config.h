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

/*
 * 获取产品公共地址(广播地址).公共地址后4字节为产品ID的CRC32,其余字节填充0xFF
 */
bool product_config_public_channel_addr(uint8_t *addr,size_t addr_len);


/*
 * 获取产品私有地址，私有地址后4字节为UID（唯一ID）的CRC32，其余字节填充0x00(可用于分配子地址，不能修改为0xFF).
 */
bool product_config_private_channel_addr(uint8_t *addr,size_t addr_len,const uint8_t *uid,size_t uid_len);

/*
 * Chacha20加密/解密。内部采用
 *     counter 计数器初值,一般使用帧中的序号决定
 *     nonce(nonce_len) 一次性数字，一般由建立连接时双方协商(最高96位),默认情况下使用公共地址填充未使用的字节。
 */
bool product_config_data_chacha20_crypto(uint8_t *data,size_t data_len,uint32_t counter,const uint8_t *nonce,size_t nonce_len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
