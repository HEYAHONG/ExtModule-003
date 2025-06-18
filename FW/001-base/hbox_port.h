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

#endif
