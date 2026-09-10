#ifndef __HBOX_PORT_H__
#define __HBOX_PORT_H__

#define HDEFAULTS_MUTEX_LOCK                                hbox_mutex_lock
#define HDEFAULTS_MUTEX_UNLOCK                              hbox_mutex_unlock
#define HDEFAULTS_TICK_GET                                  hbox_tick_get
#define HDEFAULTS_SYSCALL_NO_IMPLEMENTATION                 1

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
#define HPUTCHAR hbox_console_putchar

/*
 * 使用自定义的getchar
 */
#define HGETCHAR hbox_console_getchar

/*
 * 配置openblt
 */
#define HOPENBLT_NVM_NO_IMPLEMENTATION 1
#define BOOT_COM_RS232_ENABLE 1
#define BOOT_COM_RS232_BAUDRATE (115200)
#define BOOT_NVM_SIZE_KB 8192

#endif
