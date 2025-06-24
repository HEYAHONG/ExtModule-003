
/*
 * 启用半主机模式
 */
__asm (".global __use_no_semihosting\n\t");  

void _sys_exit(int x)
{
    x=x;
}

void _ttywrch(int ch)
{

}

const char * _sys_command_string()
{
    return "arm";
}
