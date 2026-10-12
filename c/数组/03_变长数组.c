/*
用途：演示运行时确定长度的局部变长数组（VLA）及安全长度检查。
输入：一个 1 到 100 之间的整数，作为数组长度。
输出：VLA 的元素数、字节数和逐项填入的值。
编译：gcc -std=c11 -Wall -Wextra -Wpedantic 03_变长数组.c -o array_vla
运行：./array_vla
*/
#include <stdio.h>

/* 当实现声明不支持 VLA 时，选择下面不声明变长数组的入口函数。 */
#if defined(__STDC_NO_VLA__) && __STDC_NO_VLA__

/*
用途：在当前 C 实现不支持 VLA 时，说明能力限制并正常结束。
参数：无（main 的参数列表为 void）。
返回值：提示输出后返回 0。
异常/错误：C 不使用异常机制；预处理宏已检测不支持情况，本分支只给出说明，不尝试声明 VLA。
副作用：向 stdout 输出一条功能提示。
复杂度：时间 O(1)，辅助空间 O(1)，不分配数组。
示例：当 __STDC_NO_VLA__ 已定义且值非零时，程序入口执行 puts("当前 C 实现声明不支持变长数组。")。
*/
int main(void)
{
    puts("当前 C 实现声明不支持变长数组。");
    return 0;
}

#else

/*
用途：读取合法数组长度，创建 VLA，演示运行时长度及逐项赋值。
参数：无（main 的参数列表为 void）。
返回值：长度读取、范围检查均通过时返回 0；读取失败或长度不在 1 到 100 时返回 1。
异常/错误：C 不使用异常机制；在声明 VLA 前检查 scanf 转换结果和长度范围，失败时向 stderr 报错并返回 1。
副作用：向 stdout 输出提示、长度、字节数和数组元素；错误时向 stderr 输出诊断；从 stdin 读取一个整数。
复杂度：令 n 为合法输入的数组长度（1≤n≤100）；填充和打印用时 O(n)，局部 VLA 占用 O(n) 空间，其他辅助空间 O(1)。
示例：运行后输入“5”，main 创建 5 个 int 的 VLA，并填入、打印 1 到 5。
*/
int main(void)
{
    /* count 的单位是元素个数；在通过 [1, 100] 检查前不用于 VLA 声明。 */
    int count;

    puts("请输入数组长度（1 到 100）：");
    if (scanf("%d", &count) != 1) {
        fputs("输入失败：长度必须是整数。\n", stderr);
        return 1;
    }
    /* VLA 长度必须为正；同时限制上界，避免本例创建过大的局部数组。 */
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
    /* allocated_count 保存创建时的元素数；bytes_at_creation 的单位为字节。 */
    int allocated_count = count;
    size_t bytes_at_creation = sizeof values;

    /* 仅在 count 大于 1 时递减，展示普通变量变化不会改变已创建 VLA 的长度，也不让 count 变成 0。 */
    if (count > 1) {
        --count;
    }
    printf("创建时长度：%d；随后 count 变量的值：%d\n",
           allocated_count, count);
    printf("数组占用字节数：%lu\n", (unsigned long)bytes_at_creation);

    /* 使用创建时保存的长度遍历 values；i 的合法下标范围为 [0, allocated_count)。 */
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
