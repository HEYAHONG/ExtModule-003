#ifndef __HBOX_PORT_H__
#define __HBOX_PORT_H__

#define HDEFAULTS_MUTEX_LOCK                                hbox_mutex_lock
#define HDEFAULTS_MUTEX_UNLOCK                              hbox_mutex_unlock
#define HDEFAULTS_TICK_GET                                  hbox_tick_get
#define USING_HMEMORYHEAP                                   1
#define HMEMORYHEAP_DEFAULT_POOL_SIZE                       1024

//#define HDEFAULTS_SYSCALL_NO_IMPLEMENTATION                 1

/*
 * ����C++��ʼ��
 */
#define HCPPRT_USE_CTORS       1


/*
 * ���ó�ʼ����
 */
#define HRUNTIME_USING_INIT_SECTION 1

/*
 * ����ѭ����
 */
#define HRUNTIME_USING_LOOP_SECTION 1

/*
 * ���÷��Ŷ�
 */
#define HRUNTIME_USING_SYMBOL_SECTION 1

/*
 * LZ4ʹ����С�ڴ�(1KB)
 */
#define LZ4_MEMORY_USAGE LZ4_MEMORY_USAGE_MIN

/*
 * �Զ���abort
 */
#define HABORT hbox_abort

/*
 * �Զ���exit
 */
#define HEXIT hbox_exit

/*
 * ʹ���Զ����putchar
 */
#define HPUTCHAR hbox_shell_putchar

/*
 * ʹ���Զ����getchar
 */
#define HGETCHAR hbox_shell_getchar

/*
 * ʹ���Զ����fwrite
 */
#define HFWRITE hbox_fwrite

/*
 * ������argtable3
 */
#define H3RDPARTY_USING_SYSTEM_ARGTABLE3 1

/*
 * �رղ���printf���ܽ�ʡ�ռ�
 */
#define PRINTF_DISABLE_SUPPORT_FLOAT       1
#define PRINTF_DISABLE_SUPPORT_EXPONENTIAL 1
#define PRINTF_DISABLE_SUPPORT_PTRDIFF_T   1

/*
 * Ĭ����������ڴ�
 */
struct hsoftwarevirtualmemory_map_item;
extern const struct hsoftwarevirtualmemory_map_item * const hbox_vm_map;

#endif
