#ifndef __PRODUCT_CONFIG_H__
#define __PRODUCT_CONFIG_H__
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "main.h"
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
 * ��Ʒ����xl2400p�Ĵ������
 */
#ifndef PRODUCT_XL2400P_REGISTER_CHECK_ENABLE
#define PRODUCT_XL2400P_REGISTER_CHECK_ENABLE 0
#endif

/*
 * ��Ʒ����console(0=��ʹ��,1=ʹ��)
 */
#ifndef PRODUCT_CONSOLE_ENABLE
#define PRODUCT_CONSOLE_ENABLE 1
#endif

/*
 * ��Ʒ����SHELL(0=��ʹ��,1=ʹ��)
 */
#ifndef PRODUCT_SHELL_ENABLE
#define PRODUCT_SHELL_ENABLE 0
#endif

/*
 * ��Ʒ����UART2�㲥���ݰ�͸��(�����ڲ���)
 */
#ifndef PRODUCT_UART2_BOARDCAST
#define PRODUCT_UART2_BOARDCAST 1
#endif

/*
 * ��ȡ��ƷID(ͨ����Ʒ���Ƽ������),ͨ��128λ��
 * ���������ֲ�ͬ�����硣
 */
const uint8_t * product_config_product_id_get(size_t *product_id_len);

/*
 * ��ȡ��Ʒ��Կ(ͨ����ƷID��),ͨ��256λ��
 * ������ͨ�Ź�����ʹ��AES����ChaCha20���ܵ���Կ��
 */
const uint8_t * product_config_product_key_get(size_t *product_key_len);

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

/*
 * ����ѹ��������ѹ����Ĵ�С(0��ʾʧ��)
 * ���������д������ظ�����ʱ���Ƽ�ͨ��ѹ�������ظ�����(Ҳ�ɼ��ټ���ͨ��ʱ��Կ��й¶�ķ���)��ע��:ʹ��ѹ��ʱ��ȷ������ʣ��1.5KB��ջ�ռ�
 */
size_t product_config_data_compress(void *dst,size_t dst_len,const void *src,size_t src_len);


/*
 * ���ݳ���ѹ������ѹ��ʧ��ʱ����ѹ�����С����ԭ��Сʱ������Դ���ݵ�Ŀ�껺����
 * Ҫ��ͬproduct_config_data_compress
 */
size_t product_config_data_try_compress(void *dst,size_t dst_len,const void *src,size_t src_len);


/*
 * ���ݽ�ѹ�������ؽ�ѹ����Ĵ�С(0��ʾʧ��)
 */
size_t product_config_data_uncompress(void *dst,size_t dst_len,const void *src,size_t src_len);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif
