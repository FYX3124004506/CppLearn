/*
 * 文件用途：展示函数调用链、递归、分步求值、strlen 返回值和 printf 的字符计数。
 * 输入：无；所有调用参数都是固定值。
 * 输出：嵌套调用与递归结果、分步调用结果、字符串长度和 printf 返回的字符数。
 * 独立运行：gcc -std=c11 -Wall -Wextra -Wpedantic 嵌套调用与printf返回.c -o 嵌套调用与printf返回.exe
 */
#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <stddef.h>
#include <stdio.h>
#include <string.h>

/*
 * 用途：把输入整数乘以 2。
 * 参数：value 是 int；本例要求乘积可由 int 表示。
 * 返回值：value 的两倍。
 * 异常/错误：C 不使用异常；不检查溢出，调用者须满足范围前提。
 * 副作用：无。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 示例：main 在 add_one(double_value(4)) 中先调用本函数，得到 8。
 */
int double_value(int value);

/*
 * 用途：把输入整数加 1。
 * 参数：value 是 int；本例要求加 1 后仍可由 int 表示。
 * 返回值：value 加 1 的结果。
 * 异常/错误：C 不使用异常；不检查溢出，调用者须满足范围前提。
 * 副作用：无。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 示例：main 用它包住 double_value(4)，整个调用链得到 9。
 */
int add_one(int value);

/* 在 main 前声明递归函数，具体实现位于文件后部。 */
unsigned int sum_to_n(unsigned int n);

/*
 * 用途：运行嵌套函数调用、递归、显式分步调用以及标准库返回值观察。
 * 参数：无；所有输入固定在函数体内。
 * 返回值：int；输出成功返回 0，遇到 printf 输出错误返回 1。
 * 异常/错误：C 不使用异常；检查关键 printf 调用的负返回值。
 * 副作用：向标准输出写入结果。
 * 复杂度：时间 O(n)，n 为固定字符串长度及固定输出字符数；辅助空间 O(1)。
 * 示例：启动程序后，观察嵌套结果 9、递归结果 15、字符串长度 6 和 printf("43") 返回 2。
 */
int main(void)
{
    int nested_result = add_one(double_value(4));
    int first_value = double_value(2);
    int second_value = add_one(2);
    int combined_value = first_value + second_value;
    unsigned int recursion_result = sum_to_n(5U);
    size_t text_length = strlen("abcdef");
    int written_count;

    if (printf("nested result: %d\n", nested_result) < 0) {
        return 1;
    }

    if (printf("sum_to_n(5) = %u\n", recursion_result) < 0) {
        return 1;
    }

    /*
     * 分开的语句明确先后顺序。不要依赖同一次函数调用的多个实参
     * 从左到右求值；C 不规定它们必须按该顺序执行。
     */
    if (printf("separate calls: %d + %d = %d\n",
               first_value, second_value, combined_value) < 0) {
        return 1;
    }

    if (printf("strlen(\"abcdef\") = %zu\n", text_length) < 0) {
        return 1;
    }

    written_count = printf("43");
    if (written_count < 0) {
        return 1;
    }
    if (printf(" -> printf returned %d characters\n", written_count) < 0) {
        return 1;
    }

    return 0;
}

/*
 * 用途：将 int 输入乘以 2。
 * 参数：value 是输入整数；调用者保证结果可由 int 表示。
 * 返回值：两倍的整数值。
 * 异常/错误：C 不使用异常；没有运行时错误分支。
 * 副作用：无。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 示例：main 调用 double_value(4)，返回 8。
 */
int double_value(int value)
{
    return value * 2;
}

/*
 * 用途：将 int 输入加 1。
 * 参数：value 是输入整数；调用者保证结果可由 int 表示。
 * 返回值：加 1 后的整数值。
 * 异常/错误：C 不使用异常；没有运行时错误分支。
 * 副作用：无。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 示例：main 调用 add_one(double_value(4))，接收 9。
 */
int add_one(int value)
{
    return value + 1;
}

/*
 * 用途：用基准情况与规模递减的递归调用计算 1 到 n 的和。
 * 参数：n 为 0 到 100 的无符号整数；超出该约定的调用不属于示例保证范围。
 * 返回值：从 1 到 n 的连续整数和；n 为 0 时返回 0。
 * 异常/错误：C 不使用异常；没有运行时错误分支，调用者须满足参数范围。
 * 副作用：无。
 * 复杂度：时间 O(n)，递归调用栈辅助空间 O(n)，n 为参数值。
 * 示例：main 调用 sum_to_n(5U)，基准情况最终使调用返回 15。
 */
unsigned int sum_to_n(unsigned int n)
{
    if (n == 0U) {
        return 0U;
    }
    return n + sum_to_n(n - 1U);
}
