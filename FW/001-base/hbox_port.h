#ifndef __HBOX_PORT_H__
#define __HBOX_PORT_H__

#define HDEFAULTS_MUTEX_LOCK                                hbox_mutex_lock
#define HDEFAULTS_MUTEX_UNLOCK                              hbox_mutex_unlock
#define HDEFAULTS_TICK_GET                                  hbox_tick_get
#define USING_HMEMORYHEAP                                   1
#define HMEMORYHEAP_DEFAULT_POOL_SIZE                       1024

//#define HDEFAULTS_SYSCALL_NO_IMPLEMENTATION                 1

/*
 * 启用C++初始化
 */
#define HCPPRT_USE_CTORS       1


/*
 * 启用初始化段
 */
#define HRUNTIME_USING_INIT_SECTION 1

/*
 * 启用循环段
 */
#define HRUNTIME_USING_LOOP_SECTION 1

/*
 * 启用符号段
 */
#define HRUNTIME_USING_SYMBOL_SECTION 1

/*
 * LZ4使用最小内存(1KB)
 */
#define LZ4_MEMORY_USAGE LZ4_MEMORY_USAGE_MIN

/*
 * 自定义abort
 */
#define HABORT hbox_abort

/*
 * 自定义exit
 */
#define HEXIT hbox_exit

/*
 * 使用自定义的putchar
 */
#define HPUTCHAR hbox_shell_putchar

/*
 * 使用自定义的getchar
 */
#define HGETCHAR hbox_shell_getchar

/*
 * 使用自定义的fwrite
 */
#define HFWRITE hbox_fwrite

/*
 * 不启用argtable3
 */
#define H3RDPARTY_USING_SYSTEM_ARGTABLE3 1

/*
 * 关闭部分printf功能节省空间
 */
#define PRINTF_DISABLE_SUPPORT_FLOAT       1
#define PRINTF_DISABLE_SUPPORT_EXPONENTIAL 1
#define PRINTF_DISABLE_SUPPORT_PTRDIFF_T   1

/*
 * 默认软件虚拟内存
 */
struct hsoftwarevirtualmemory_map_item;
extern const struct hsoftwarevirtualmemory_map_item * const hbox_vm_map;

#endif
