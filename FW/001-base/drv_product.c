#include "product_config.h"
#include "hbox.h"

#ifdef HRUNTIME_USING_INIT_SECTION
void  hproduct_init(const hruntime_function_t *func)
{
    //�ڴ˴����м���ɼ���ջ����Ŀ����ԡ�
    product_config_product_id_get(NULL);
    product_config_product_key_get(NULL);
}
HRUNTIME_INIT_EXPORT(product,0,hproduct_init,NULL);
#endif
#ifdef HRUNTIME_USING_LOOP_SECTION
void  hproduct_loop(const hruntime_function_t *func)
{

}
HRUNTIME_LOOP_EXPORT(product,0,hproduct_loop,NULL);
#endif

