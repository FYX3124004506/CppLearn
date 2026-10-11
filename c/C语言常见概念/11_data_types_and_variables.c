/* =========================================================
 * 文件名：11_data_types_and_variables.c
 * 描述：C 语言基本类型、变量初始化、sizeof 和常见运算示例
 * ========================================================= */

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

int global_count; /* 具有静态存储期，未显式初始化时为零。 */

int main(void)
{
    char initial = 'A';
    unsigned char small_value = 200;
    int count = 5;
    long population = 123456L;
    float rate = 0.75f;
    double price = 12.5;
    bool is_ready = true;

    printf("字符：%c，unsigned char：%u\n",
           initial, (unsigned int)small_value);
    printf("整数：%d，long：%ld\n", count, population);
    printf("浮点数：%.2f，double：%.2f\n", rate, price);
    printf("布尔值：%s\n", is_ready ? "true" : "false");

    /* 兼容部分 MinGW 格式诊断；这里转换已知较小的类型大小。 */
    printf("sizeof(char) = %lu byte\n", (unsigned long)sizeof(char));
    printf("sizeof(int) = %lu bytes\n", (unsigned long)sizeof(int));
    printf("sizeof(double) = %lu bytes\n", (unsigned long)sizeof(double));
    printf("一个字节含 %d bit\n", CHAR_BIT);
    printf("静态存储期变量的默认值：%d\n", global_count);

    printf("整数除法：%d / 2 = %d\n", count, count / 2);
    printf("浮点除法：%d / 2.0 = %.1f\n", count, count / 2.0);
    printf("浮点数转整数：%.2f -> %d\n", price, (int)price);

    count += 3;
    {
        int old_count = count++;
        int new_count = ++count;
        printf("复合赋值和自增：count=%d，后置结果=%d，前置结果=%d\n",
               count, old_count, new_count);
    }

    return 0;
}
