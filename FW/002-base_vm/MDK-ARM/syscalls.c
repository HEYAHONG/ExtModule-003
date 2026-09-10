#include "rt_sys.h"
#include "string.h"
/*
 * 关闭半主机模式
 */
#ifdef __clang__
__asm(".global __use_no_semihosting\n\t");
#else
#pragma import(__use_no_semihosting_swi)
#endif

/* Standard IO device handles. */
#define STDIN       0
#define STDOUT      1
#define STDERR      2

/* Standard IO device name defines. */
const char __stdin_name[]  = "STDIN";
const char __stdout_name[] = "STDOUT";
const char __stderr_name[] = "STDERR";


/**
 * required by fopen() and freopen().
 *
 * @param name - file name with path.
 * @param openmode - a bitmap hose bits mostly correspond directly to
 *                     the ISO mode specification.
 * @return  -1 if an error occurs.
 */
FILEHANDLE $Sub$$_sys_open(const char *name, int openmode)
{
    /* Register standard Input Output devices. */
    if (strcmp(name, __stdin_name) == 0)
        return (STDIN);
    if (strcmp(name, __stdout_name) == 0)
        return (STDOUT);
    if (strcmp(name, __stderr_name) == 0)
        return (STDERR);


    return -1;
}

/*
 * Read from a file. Can return:
 *  - zero if the read was completely successful
 *  - the number of bytes _not_ read, if the read was partially successful
 *  - the number of bytes not read, plus the top bit set (0x80000000), if
 *    the read was partially successful due to end of file
 *  - -1 if some error other than EOF occurred
 *
 * It is also legal to signal EOF by returning no data but
 * signalling no error (i.e. the top-bit-set mechanism need never
 * be used).
 *
 * So if (for example) the user is trying to read 8 bytes at a time
 * from a file in which only 5 remain, this routine can do three
 * equally valid things:
 *
 *  - it can return 0x80000003 (3 bytes not read due to EOF)
 *  - OR it can return 3 (3 bytes not read), and then return
 *    0x80000008 (8 bytes not read due to EOF) on the next attempt
 *  - OR it can return 3 (3 bytes not read), and then return
 *    8 (8 bytes not read, meaning 0 read, meaning EOF) on the next
 *    attempt
 *
 * `mode' exists for historical reasons and must be ignored.
 */
int $Sub$$_sys_read(FILEHANDLE fh, unsigned char *buf, unsigned len, int mode)
{
    return 0;
}

/*
 * Write to a file. Returns 0 on success, negative on error, and
 * the number of characters _not_ written on partial success.
 * `mode' exists for historical reasons and must be ignored.
 * The return value is either:
 * A positive number representing the number of characters not written
 * (so any nonzero return value denotes a failure of some sort).
 * A negative number indicating an error.
 */
int $Sub$$_sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode)
{
    return 0;
}

/*
 * Flush any OS buffers associated with fh, ensuring that the file
 * is up to date on disk. Result is >=0 if OK, negative for an
 * error.
 * This function is deprecated. It is never called by any other library function,
 * and you are not required to re-implement it if you are retargeting standard I/O (stdio).
 */
int $Sub$$_sys_ensure(FILEHANDLE fh)
{
    return -1;
}

/*
 * Move the file position to a given offset from the file start.
 * Returns >=0 on success, <0 on failure.
 */
int $Sub$$_sys_seek(FILEHANDLE fh, long pos)
{
    return -1;
}

/**
 * used by tmpnam() or tmpfile()
 */
#if __ARMCC_VERSION >= 6190000
void $Sub$$_sys_tmpnam(char *name, int fileno, unsigned maxlength)
{

}
#else
int $Sub$$_sys_tmpnam(char *name, int fileno, unsigned maxlength)
{
    return 0;
}
#endif /* __ARMCC_VERSION >= 6190000 */

char *$Sub$$_sys_command_string(char *cmd, int len)
{
    return "arm";
}

/* This function writes a character to the console. */
void $Sub$$_ttywrch(int ch)
{

}

void $Sub$$_sys_exit(int return_code)
{

}

/**
 * return current length of file.
 *
 * @param fh - file handle
 * @return file length, or -1 on failed
 */
long $Sub$$_sys_flen(FILEHANDLE fh)
{
    return -1;
}

int $Sub$$_sys_istty(FILEHANDLE fh)
{
    return 0;
}

int $Sub$$remove(const char *filename)
{
    return -1;
}