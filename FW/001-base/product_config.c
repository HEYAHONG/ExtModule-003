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

/*
 * SOC/SOLUTION名称
 */
#ifndef PRODUCT_SOC_NAME_OR_SOLUTION_NAME
#define PRODUCT_SOC_NAME_OR_SOLUTION_NAME XL2409
#endif

/*
 * 产品SOC名称
 */
#define PRODUCT_SOC_NAME_OR_SOLUTION_NAME_STRING hdefaults_xstr(PRODUCT_SOC_NAME_OR_SOLUTION_NAME)

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
        hhmac_md5_starts(&ctx,(const uint8_t *)PRODUCT_SOC_NAME_OR_SOLUTION_NAME_STRING,strlen(PRODUCT_SOC_NAME_OR_SOLUTION_NAME_STRING));
        hhmac_md5_update(&ctx,(const uint8_t *)PRODUCT_NAME_STRING,strlen(PRODUCT_NAME_STRING));
        hhmac_md5_finish(&ctx);
        memcpy(product_id,ctx.hash_result,sizeof(product_id));
    }

    //格式化为UUID
    huuid_custom_uuid_format((uint8_t *)product_id);

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

bool product_config_public_channel_addr(uint8_t *addr,size_t addr_len)
{
    bool ret=false;
    if(addr==NULL || addr_len==0)
    {
        return ret;
    }
    ret=true;
    memset(addr,0xFF,addr_len);
    uint32_t crc=0;
    {
        size_t id_len=0;
        crc=hcrc_crc32_fast_calculate(product_config_product_id_get(&id_len),id_len);
    }
    if(addr_len > 1)
    {
        addr[addr_len-1]=((crc>>24)&0xFF);
    }
    if(addr_len > 2)
    {
        addr[addr_len-2]=((crc>>16)&0xFF);
    }
    if(addr_len > 3)
    {
        addr[addr_len-3]=((crc>>8)&0xFF);
    }
    if(addr_len > 4)
    {
        addr[addr_len-4]=((crc>>0)&0xFF);
    }
    return ret;
}

bool product_config_private_channel_addr(uint8_t *addr,size_t addr_len,const uint8_t *uid,size_t uid_len)
{
    bool ret=false;
    if(addr==NULL || addr_len==0 || uid==NULL || uid_len==0)
    {
        return ret;
    }
    ret=true;
    memset(addr,0x0,addr_len);
    uint32_t crc=0;
    {
        crc=hcrc_crc32_fast_calculate(uid,uid_len);
    }
    if(addr_len > 1)
    {
        addr[addr_len-1]=((crc>>24)&0xFF);
    }
    if(addr_len > 2)
    {
        addr[addr_len-2]=((crc>>16)&0xFF);
    }
    if(addr_len > 3)
    {
        addr[addr_len-3]=((crc>>8)&0xFF);
    }
    if(addr_len > 4)
    {
        addr[addr_len-4]=((crc>>0)&0xFF);
    }
    return ret;
}

bool product_config_data_chacha20_crypto(uint8_t *data,size_t data_len,uint32_t counter,const uint8_t *nonce,size_t nonce_len)
{
    bool ret=false;
    if(data==NULL || data_len==0 )
    {
        return ret;
    }
    hchacha20_context_t ctx= {0};
    hchacha20_setkey(&ctx,product_config_product_key_get(NULL));
    {
        uint8_t nonce_temp[12]= {0};
        product_config_public_channel_addr(nonce_temp,sizeof(nonce_temp));
        if(nonce_len > sizeof(nonce_temp))
        {
            nonce_len=sizeof(nonce_temp);
        }
        if(nonce!=NULL && nonce_len != 0)
        {
            memcpy(nonce_temp,nonce,nonce_len);
        }
        hchacha20_starts(&ctx,nonce,counter);
    }
    hchacha20_update(&ctx,data_len,data,data);
    ret=true;
    return ret;
}
