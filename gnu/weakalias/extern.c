#include <stdio.h>

int weak_var_test = 6666;   /* 已初始化的全局变量，属于强符号 */
void weak_func_test(void)   /* 全局函数属于强符号 */
{
    printf("strong symbol in %s:%s() in\n", __FILE__, __func__);
}
