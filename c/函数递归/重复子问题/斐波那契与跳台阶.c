/*
 * 文件用途：对比斐波那契数和青蛙跳台阶的朴素递归、记忆化与循环计算。
 * 输入：无；程序使用 n=10 的斐波那契示例，以及 0、1、5 级台阶示例。
 * 输出：结果、递归调用次数和不同写法的计算结果。
 * 独立编译运行：gcc -std=c11 -Wall -Wextra -Wpedantic 斐波那契与跳台阶.c -o repeated_subproblems.exe
 */

#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <stdbool.h>
#include <stdio.h>

/*
 * 用途：直接按 F(n)=F(n-1)+F(n-2) 递归计算斐波那契数。
 * 参数：n 为 0 到 10；calls 指向有效的调用计数器。
 * 返回值：F(n)。
 * 异常/错误：C 不使用异常；无错误返回，调用者须满足参数范围并提供有效指针。
 * 副作用：每次进入函数都递增 *calls。
 * 复杂度：n 为参数值，时间 O(2^n)，最大递归深度和辅助空间 O(n)。
 * 用法示例：main 传入 n=10 和初值为 0 的计数器，得到 55 和 177 次调用。
 */
static unsigned long long fibonacci_naive(
    unsigned int n,
    unsigned long long *calls)
{
    ++(*calls);

    if (n <= 1U) {
        return n;
    }

    return fibonacci_naive(n - 1U, calls)
         + fibonacci_naive(n - 2U, calls);
}

/*
 * 用途：用缓存避免重复计算斐波那契子问题。
 * 参数：n 为 0 到 10；cache 与 known 均指向长度至少为 11 的数组；
 *       calls 指向有效计数器，且数组元素在首次调用前初始化。
 * 返回值：F(n)。
 * 异常/错误：C 不使用异常；无错误返回，调用者须满足数组长度、初始化和指针前提。
 * 副作用：递增 *calls，并写入 cache 与 known 中已计算的结果。
 * 复杂度：n 为参数值，时间 O(n)；递归栈 O(n)，缓存由调用者提供，占 O(n) 空间。
 * 用法示例：main 用全零 cache 和全 false known 调用 n=10，结果为 55。
 */
static unsigned long long fibonacci_memoized(
    unsigned int n,
    unsigned long long cache[11],
    bool known[11],
    unsigned long long *calls)
{
    ++(*calls);

    if (known[n]) {
        return cache[n];
    }

    if (n <= 1U) {
        cache[n] = n;
        known[n] = true;
        return cache[n];
    }

    cache[n] = fibonacci_memoized(n - 1U, cache, known, calls)
             + fibonacci_memoized(n - 2U, cache, known, calls);
    known[n] = true;
    return cache[n];
}

/*
 * 用途：用循环从小到大计算斐波那契数。
 * 参数：n 为 0 到 10。
 * 返回值：F(n)。
 * 异常/错误：C 不使用异常；无错误返回，调用者须遵守参数范围。
 * 副作用：无。
 * 复杂度：n 为参数值，时间 O(n)，辅助空间 O(1)。
 * 用法示例：main 调用 fibonacci_iterative(10U)，得到 55。
 */
static unsigned long long fibonacci_iterative(unsigned int n)
{
    /* 只保留相邻两项；每轮用 next 推进到下一个数。 */
    unsigned long long previous = 0ULL;
    unsigned long long current = 1ULL;

    if (n == 0U) {
        return 0ULL;
    }

    for (unsigned int index = 2U; index <= n; ++index) {
        const unsigned long long next = previous + current;
        previous = current;
        current = next;
    }

    return current;
}

/*
 * 用途：按青蛙每次跳 1 级或 2 级的规则，直接递归计算到达 n 级的方法数。
 * 参数：n 为 0 到 10；calls 指向有效的调用计数器。
 * 返回值：ways(n)，其中 ways(0)=1、ways(1)=1。
 * 异常/错误：C 不使用异常；无错误返回，调用者须满足参数范围并提供有效指针。
 * 副作用：每次进入函数都递增 *calls。
 * 复杂度：n 为台阶数，时间 O(2^n)，最大递归深度和辅助空间 O(n)。
 * 用法示例：main 传入 n=5，得到 8 种走法。
 */
static unsigned long long frog_ways_naive(
    unsigned int n,
    unsigned long long *calls)
{
    ++(*calls);

    if (n <= 1U) {
        return 1ULL;
    }

    return frog_ways_naive(n - 1U, calls)
         + frog_ways_naive(n - 2U, calls);
}

/*
 * 用途：用循环计算每次可跳 1 级或 2 级时到达 n 级的方法数。
 * 参数：n 为 0 到 10。
 * 返回值：ways(n)，其中空路径 ways(0)=1。
 * 异常/错误：C 不使用异常；无错误返回，调用者须遵守参数范围。
 * 副作用：无。
 * 复杂度：n 为台阶数，时间 O(n)，辅助空间 O(1)。
 * 用法示例：main 调用 frog_ways_iterative(5U)，得到 8。
 */
static unsigned long long frog_ways_iterative(unsigned int n)
{
    /* previous 和 current 分别保存相邻两个台阶数对应的方法数。 */
    unsigned long long previous = 1ULL;
    unsigned long long current = 1ULL;

    if (n <= 1U) {
        return 1ULL;
    }

    for (unsigned int level = 2U; level <= n; ++level) {
        const unsigned long long next = previous + current;
        previous = current;
        current = next;
    }

    return current;
}

/*
 * 用途：比较朴素递归、记忆化、循环，以及青蛙跳台阶的基准值。
 * 参数：无；void 表示无命令行输入，所有数据固定在 main 中。
 * 返回值：示例运行结束时返回 0。
 * 异常/错误：C 不使用异常；固定输入满足函数前提，标准输出错误未检查。
 * 副作用：向标准输出写入计算结果和调用计数。
 * 复杂度：n 为最大固定递归输入 10，时间 O(2^n)，辅助空间 O(n)。
 * 用法示例：直接启动程序，观察 F(10)=55、朴素调用 177 次、记忆化调用 19 次。
 */
int main(void)
{
    const unsigned int n = 10U;
    unsigned long long naive_calls = 0ULL;
    const unsigned long long naive_result =
        fibonacci_naive(n, &naive_calls);

    /* known 区分“未计算”和“结果为 0”，缓存最多保存 F(0) 到 F(10)。 */
    unsigned long long cache[11] = {0ULL};
    bool known[11] = {false};
    unsigned long long memo_calls = 0ULL;
    const unsigned long long memo_result =
        fibonacci_memoized(n, cache, known, &memo_calls);
    const unsigned long long iterative_result =
        fibonacci_iterative(n);

    printf("F(%u) 朴素递归：%llu，调用 %llu 次\n",
           n, naive_result, naive_calls);
    printf("F(%u) 记忆化：%llu，调用 %llu 次\n",
           n, memo_result, memo_calls);
    printf("F(%u) 循环：%llu\n", n, iterative_result);

    unsigned long long base_calls = 0ULL;
    const unsigned long long fibonacci_zero =
        fibonacci_naive(0U, &base_calls);
    base_calls = 0ULL;
    const unsigned long long fibonacci_one =
        fibonacci_naive(1U, &base_calls);
    printf("递归基准：F(0)=%llu，F(1)=%llu\n",
           fibonacci_zero, fibonacci_one);

    unsigned long long frog_calls = 0ULL;
    printf("跳 0 级：%llu 种\n", frog_ways_naive(0U, &frog_calls));
    frog_calls = 0ULL;
    printf("跳 1 级：%llu 种\n", frog_ways_naive(1U, &frog_calls));
    frog_calls = 0ULL;
    const unsigned long long frog_recursive_result =
        frog_ways_naive(5U, &frog_calls);
    printf("跳 5 级递归：%llu 种，调用 %llu 次\n",
           frog_recursive_result, frog_calls);
    printf("跳 5 级循环：%llu 种\n", frog_ways_iterative(5U));

    return 0;
}
