#include <stdio.h>

void weak_func_test(void) __attribute__((weak));
    /*
     *显式申明为weak，属于弱符号(函数)
     *将本模块(main.c)中的weak_func_test 声明为弱符号
     */
int  weak_var_test;				    /* 未初始化的全局变量，属于弱符号 */

#ifdef WEAK_SYM
void weak_func_test(void)
{
    printf("weak symbol in %s:%s.c in\n", __FILE__, __func__);
}
#endif

int main()
{
    printf("weak_var_test:%d\n", weak_var_test);
    weak_func_test();
    return 0;
}
