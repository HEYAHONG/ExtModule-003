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

#endif
