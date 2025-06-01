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

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
