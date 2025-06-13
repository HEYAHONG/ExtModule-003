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
 * ��Ʒ����
 */
#ifdef  PRODUCT_NAME
#define PRODUCT_NAME ExtModule_003
#endif

/*
 * ��Ʒ����
 */
#ifdef  PRODUCT_PSK
#define PRODUCT_NAME ExtModule_003_PSK
#endif

/*
 * ��ȡ��ƷID(ͨ����Ʒ���Ƽ������),ͨ��128λ��
 * ���������ֲ�ͬ�����硣
 */
const uint8_t *	product_config_product_id_get(size_t *product_id_len);

/*
 * ��ȡ��Ʒ��Կ(ͨ����ƷID��),ͨ��256λ��
 * ������ͨ�Ź�����ʹ��AES����ChaCha20���ܵ���Կ��
 */
const uint8_t *	product_config_product_key_get(size_t *product_key_len);

/*
 * ��ȡ��Ʒ������ַ(�㲥��ַ).������ַ��4�ֽ�Ϊ��ƷID��CRC32,�����ֽ����0xFF
 */
bool product_config_public_channel_addr(uint8_t *addr,size_t addr_len);


/*
 * ��ȡ��Ʒ˽�е�ַ��˽�е�ַ��4�ֽ�ΪUID��ΨһID����CRC32�������ֽ����0x00(�����ڷ����ӵ�ַ�������޸�Ϊ0xFF).
 */
bool product_config_private_channel_addr(uint8_t *addr,size_t addr_len,const uint8_t *uid,size_t uid_len);

/*
 * Chacha20����/���ܡ��ڲ�����
 *     counter ��������ֵ,һ��ʹ��֡�е���ž���
 *     nonce(nonce_len) һ�������֣�һ���ɽ�������ʱ˫��Э��(���96λ),Ĭ�������ʹ�ù�����ַ���δʹ�õ��ֽڡ�
 */
bool product_config_data_chacha20_crypto(uint8_t *data,size_t data_len,uint32_t counter,const uint8_t *nonce,size_t nonce_len);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
