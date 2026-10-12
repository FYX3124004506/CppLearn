/*
 * 文件用途：计算 1! 到 10! 的累加和，并展示有副作用的函数调用，供断点、条件断点、监视和单步练习。
 * 输入：无外部输入；循环上限固定为 10。
 * 输出：0! 边界、每轮的阶乘值、累加值以及计数器调用前后的状态。
 * 独立编译：gcc -std=c11 -Wall -Wextra -Wpedantic 阶乘累加跟踪.c -o factorial_trace.exe
 * 运行：启动程序；在 sum += value 处按 F9，按 F5 运行，并观察 n、value、sum。
 * 调试：条件断点可设为 n == 5；在 factorial 调用处用 F10 越过函数、F11 进入函数。
 */
#include <stdio.h>


/*
 * 用途：递增调用者提供的计数器并返回新值，展示函数调用的可观察副作用。
 * 参数：counter 指向仍在生命周期内的 unsigned int 对象，不能为 NULL；示例初值为 0，旧值为最大值时会按无符号规则回绕。
 * 返回值：递增后的计数器值。
 * 异常/错误：C 不使用异常；不检查空指针，调用者须满足有效指针前提；无错误返回。
 * 副作用：修改 *counter；无输入输出。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 用法：main 调用 increment_and_return(&watch_counter)，观察 0 变为 1 且返回 1。
 */
static unsigned int increment_and_return(unsigned int *counter)
{
    return ++*counter;
}

/*
 * 用途：计算给定非负整数的阶乘。
 * 参数：n 是阶乘自变量；有效范围为 0 到 10，本程序传入 1 到 10。
 * 返回值：n!；n 为 0 或 1 时返回 1。
 * 异常/错误：C 不使用异常；函数不检查输入范围，超出 0 到 10 的前提时不保证结果正确。
 * 副作用：无，不读取输入或修改外部状态。
 * 复杂度：n 为输入值，时间 O(n)，辅助空间 O(1)。
 * 用法：main 在每轮调用 factorial(n)，例如 n=5 时返回 120。
 */

static unsigned long factorial(unsigned int n)
{
    unsigned long result = 1;

    for (unsigned int factor = 2; factor <= n; ++factor)
    {
        result *= factor;
    }

    return result;
}

/*
 * 用途：逐轮累加 1! 到 10!，让变量状态可以在调试器中观察。
 * 参数：无；循环上限 K 固定为 10。
 * 返回值：正常运行返回 0。
 * 异常/错误：C 不使用异常；本例无外部输入和错误分支。
 * 副作用：向标准输出打印每轮的 n、value、sum 和计数器状态；末尾调用函数递增局部计数器。
 * 复杂度：K 为阶乘个数，时间 O(K^2)，辅助空间 O(1)。
 * 用法：在 Visual Studio 中启动调试后按 F9 设置断点、F5 继续；
 *       F10 越过函数调用，F11 进入 factorial，Ctrl+F5 不附加调试器运行。
 *       在监视窗口观察 watch_counter；不要把 increment_and_return 调用作为观察表达式反复执行。
 */
int main(void)
{
    const unsigned int limit = 10;
    unsigned long sum = 0;

    printf("0! = %lu\n", factorial(0));
    for (unsigned int n = 1; n <= limit; ++n)
    {
        const unsigned long value = factorial(n);
        sum += value;
        printf("%u! = %lu, 当前总和 = %lu\n", n, value, sum);
    }

    unsigned int watch_counter = 0;
    printf("有副作用调用前：%u\n", watch_counter);
    const unsigned int call_result = increment_and_return(&watch_counter);
    printf("有副作用调用后：计数器=%u，返回值=%u\n", watch_counter, call_result);

    return 0;
}
