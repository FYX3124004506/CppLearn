/*
用途：演示运行时确定长度的局部变长数组（VLA）及安全长度检查。
输入：一个 1 到 100 之间的整数，作为数组长度。
输出：VLA 的元素数、字节数和逐项填入的值。
编译：gcc -std=c11 -Wall -Wextra -Wpedantic 03_变长数组.c -o array_vla
运行：./array_vla
*/
#include <stdio.h>

#if defined(__STDC_NO_VLA__) && __STDC_NO_VLA__

int main(void)
{
    puts("当前 C 实现声明不支持变长数组。");
    return 0;
}

#else

int main(void)
{
    int count;

    puts("请输入数组长度（1 到 100）：");
    if (scanf("%d", &count) != 1) {
        fputs("输入失败：长度必须是整数。\n", stderr);
        return 1;
    }
    if (count < 1 || count > 100) {
        fputs("长度超出允许范围；变长数组长度必须为正数。\n", stderr);
        return 1;
    }

    /*
    count 在运行时读入，所以 values 是 VLA。
    声明后即使 count 变量改变，数组长度也不会跟着改变。
    VLA 没有初始化列表；下面在读取任何元素前逐项赋值。
    */
    int values[count];
    int allocated_count = count;
    size_t bytes_at_creation = sizeof values;

    if (count > 1) {
        --count;
    }
    printf("创建时长度：%d；随后 count 变量的值：%d\n",
           allocated_count, count);
    printf("数组占用字节数：%lu\n", (unsigned long)bytes_at_creation);

    for (int i = 0; i < allocated_count; ++i) {
        values[i] = i + 1;
    }

    printf("逐项填入结果：");
    for (int i = 0; i < allocated_count; ++i) {
        printf(" %d", values[i]);
    }
    putchar('\n');

    return 0;
}

#endif
