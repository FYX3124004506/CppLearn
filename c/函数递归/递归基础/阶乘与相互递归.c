/*
 * 文件用途：展示递归基准条件、递归展开与回归、输入边界，以及直接和间接递归。
 * 输入：无；程序使用固定的阶乘输入 -1、0、5、20、21，以及小整数 4、5。
 * 输出：阶乘结果或拒绝原因、factorial 调用返回轨迹，以及奇偶判断结果。
 * 独立编译运行：gcc -std=c11 -Wall -Wextra -Wpedantic 阶乘与相互递归.c -o factorial_recursion.exe
 */

/* C 允许直接和间接递归，但不规定统一的调用深度；示例只用小输入，不触发资源耗尽。 */

#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <stdbool.h>
#include <stdio.h>

/*
 * 用途：递归计算非负整数的阶乘，可选地打印每层进入和返回的顺序。
 * 参数：n 为 0 到 20；show_trace 为 true 时输出调用轨迹，为 false 时不输出轨迹。
 * 返回值：n!；此范围内结果可由 unsigned long long 表示。
 * 异常/错误：C 不使用异常；该内部函数不做参数检查，调用者必须遵守 n 的范围前提。
 * 副作用：show_trace 为 true 时向标准输出写入轨迹，否则无副作用。
 * 复杂度：n 为参数值，递归计算 O(n)，栈空间 O(n)；若打印轨迹，输出字符量为 O(n log n)。
 * 用法示例：factorial_recursive(4, true) 返回 24，并显示从 factorial(4) 到 factorial(0) 的调用和回归。
 */
static unsigned long long factorial_recursive(unsigned int n, bool show_trace)
{
    if (show_trace) {
        printf("进入 factorial(%u)\n", n);
    }

    if (n == 0U) {
        if (show_trace) {
            puts("返回 factorial(0) = 1");
        }
        return 1ULL;
    }

    const unsigned long long result =
        n * factorial_recursive(n - 1U, show_trace);

    if (show_trace) {
        printf("返回 factorial(%u) = %llu\n", n, result);
    }

    return result;
}

/*
 * 用途：检查输入范围后计算阶乘，避免负数和过大输入进入递归。
 * 参数：number 为待计算的有符号整数；result 必须指向可写的 unsigned long long 对象。
 * 返回值：输入在 0 到 20 且 result 非空时返回 true 并写入结果，否则返回 false。
 * 异常/错误：C 不使用异常；通过 bool 返回值报告无效指针或超出示例范围的输入；失败时不写 result。
 * 副作用：成功时写入 *result；失败时无副作用。
 * 复杂度：n 为 number，时间 O(n)，递归调用栈辅助空间 O(n)。
 * 用法示例：main 传入 5 和有效结果指针，得到 true 与 120；传入 -1 或 21 得到 false。
 */
static bool factorial_checked(int number, unsigned long long *result)
{
    if (result == NULL || number < 0 || number > 20) {
        return false;
    }

    *result = factorial_recursive((unsigned int)number, false);
    return true;
}

static bool is_odd_recursive(unsigned int n);

/*
 * 用途：用间接递归判断非负整数 n 是否为偶数。
 * 参数：n 是待判断的非负整数；本程序只传 4 和 5，更大的输入须由调用者按环境控制深度。
 * 返回值：n 为偶数时返回 true，否则返回 false。
 * 异常/错误：C 不使用异常；无错误返回，调用者须保证递归深度适合运行环境。
 * 副作用：无。
 * 复杂度：n 为输入值，时间 O(n)，递归调用栈辅助空间 O(n)。
 * 用法示例：main 调用 is_even_recursive(4) 得到 true，调用 is_even_recursive(5) 得到 false。
 */
static bool is_even_recursive(unsigned int n)
{
    if (n == 0U) {
        return true;
    }

    return is_odd_recursive(n - 1U);
}

/*
 * 用途：作为 is_even_recursive 的互补函数，展示两个函数通过调用链形成间接递归。
 * 参数：n 是待判断的非负整数；本程序只传 4 和 5，更大的输入须由调用者按环境控制深度。
 * 返回值：n 为奇数时返回 true，否则返回 false。
 * 异常/错误：C 不使用异常；无错误返回，调用者须保证递归深度适合运行环境。
 * 副作用：无。
 * 复杂度：n 为输入值，时间 O(n)，递归调用栈辅助空间 O(n)。
 * 用法示例：is_even_recursive(4) 调用 is_odd_recursive(3)，再交替调用直至 n 为 0。
 */
static bool is_odd_recursive(unsigned int n)
{
    if (n == 0U) {
        return false;
    }

    return is_even_recursive(n - 1U);
}

/*
 * 用途：运行阶乘边界、递归回归轨迹和互相调用的奇偶判断。
 * 参数：无；void 表示不接收命令行参数，所有数据固定在 main 中。
 * 返回值：成功完成示例时返回 0。
 * 异常/错误：C 不使用异常；固定数据满足调用前提，标准输出错误未检查。
 * 副作用：向标准输出打印阶乘结果、调用轨迹和奇偶判断。
 * 复杂度：设 n 为最大固定递归输入 20，递归计算 O(n)、栈空间 O(n)；轨迹输出字符量 O(n log n)。
 * 用法示例：直接启动程序；程序依次展示 0、5、20、负数、21 和奇偶判断结果。
 */
int main(void)
{
    unsigned long long result = 0ULL;

    if (factorial_checked(0, &result)) {
        printf("0! = %llu\n", result);
    }
    if (factorial_checked(5, &result)) {
        printf("5! = %llu\n", result);
    }
    if (factorial_checked(20, &result)) {
        printf("20! = %llu\n", result);
    }

    if (!factorial_checked(-1, &result)) {
        puts("factorial(-1): 拒绝负数输入");
    }
    if (!factorial_checked(21, &result)) {
        puts("factorial(21): 超出本示例允许范围 0..20");
    }

    puts("调用轨迹示例 factorial(4)：");
    const unsigned long long traced_result = factorial_recursive(4U, true);
    printf("factorial(4) = %llu\n", traced_result);

    printf("4 是偶数：%s\n", is_even_recursive(4U) ? "是" : "否");
    printf("5 是偶数：%s\n", is_even_recursive(5U) ? "是" : "否");

    return 0;
}
