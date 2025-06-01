#include "product_config.h"
#include "hbox.h"

/*
 * 产品名称字符串
 */
#define PRODUCT_NAME_STRING hdefaults_xstr(PRODUCT_NAME)


/*
 * 产品名称密钥
 */
#define PRODUCT_PSK_STRING hdefaults_xstr(PRODUCT_PSK)

#ifndef PRODUCT_SOC_NAME
#define PRODUCT_SOC_NAME XL2409
#endif

/*
 * 产品SOC名称
 */
#define PRODUCT_SOC_NAME_STRING hdefaults_xstr(PRODUCT_SOC_NAME)

static hmd5_md5_t product_id= {0};
const uint8_t *	product_config_product_id_get(size_t *product_id_len)
{
    if(product_id_len!=NULL)
    {
        (*product_id_len)=sizeof(product_id);
    }
    {
        hmd5_md5_t null_md5= {0};
        if(memcmp(null_md5,product_id,sizeof(product_id))!=0)
        {
            return product_id;
        }
    }

    {
        hhmac_md5_context_t ctx= {0};
        hhmac_md5_starts(&ctx,(const uint8_t *)PRODUCT_SOC_NAME_STRING,strlen(PRODUCT_SOC_NAME_STRING));
        hhmac_md5_update(&ctx,(const uint8_t *)PRODUCT_NAME_STRING,strlen(PRODUCT_SOC_NAME_STRING));
        hhmac_md5_finish(&ctx);
        memcpy(product_id,ctx.hash_result,sizeof(product_id));
    }

    return product_id;
}


static hsha2_sha256_t product_key= {0};
const uint8_t *	product_config_product_key_get(size_t *product_key_len)
{
    if(product_key_len!=NULL)
    {
        (*product_key_len)=sizeof(product_key);
    }
    {
        hsha2_sha256_t null_sha= {0};
        if(memcmp(null_sha,product_key,sizeof(product_key))!=0)
        {
            return product_key;
        }
    }

    {
        hhmac_sha2_sha256_context_t ctx= {0};
        hhmac_sha2_sha256_starts(&ctx,(const uint8_t *)product_config_product_id_get(NULL),sizeof(product_id));
        hhmac_sha2_sha256_update(&ctx,(const uint8_t *)PRODUCT_PSK_STRING,strlen(PRODUCT_PSK_STRING));
        hhmac_sha2_sha256_finish(&ctx);
        memcpy(product_key,ctx.hash_result,sizeof(product_key));
    }

    return product_key;
}

