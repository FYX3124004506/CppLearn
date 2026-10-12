/*
 * 文件用途：并排展示递归与循环按高位输出数字，并比较尾递归和循环阶乘。
 * 输入：无；程序使用 0、1203、UINT_MAX 和 5 这些固定值。
 * 输出：数字序列、UINT_MAX 的十进制各位，以及两种阶乘结果。
 * 独立编译运行：gcc -std=c11 -Wall -Wextra -Wpedantic 数字拆分与尾递归.c -o digit_recursion.exe
 */

#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <limits.h>
#include <stdio.h>

/*
 * 用途：递归地从高位到低位输出 unsigned int 的十进制数字。
 * 参数：number 为 unsigned int 可表示的任意非负值，包括 0。
 * 返回值：无。
 * 异常/错误：C 不使用异常；整数类型已限制输入为非负值。
 * 副作用：向标准输出写入数字和空格。
 * 复杂度：d 为十进制位数，时间 O(d)，递归调用栈辅助空间 O(d)。
 * 用法示例：main 调用 print_digits_recursive(1203U)，输出 1 2 0 3。
 */
static void print_digits_recursive(unsigned int number)
{
    if (number >= 10U) {
        print_digits_recursive(number / 10U);
    }

    printf("%u ", number % 10U);
}

/*
 * 用途：用循环从高位到低位输出 unsigned int 的十进制数字。
 * 参数：number 为 unsigned int 可表示的任意非负值，包括 0。
 * 返回值：无。
 * 异常/错误：C 不使用异常；整数类型已限制输入为非负值。
 * 副作用：向标准输出写入数字和空格。
 * 复杂度：d 为十进制位数，时间 O(d)，辅助空间 O(1)。
 * 用法示例：main 调用 print_digits_iterative(1203U)，输出 1 2 0 3。
 */
static void print_digits_iterative(unsigned int number)
{
    unsigned int place_value = 1U;

    /*
     * 只有商至少为 10 时才扩大位值，所以新的 place_value 不会超过 number；
     * 对 number=0，循环不执行，后面的 do-while 仍会输出唯一的数字 0。
     */
    while (number / place_value >= 10U) {
        place_value *= 10U;
    }

    do {
        printf("%u ", number / place_value);
        number %= place_value;
        place_value /= 10U;
    } while (place_value > 0U);
}

/*
 * 用途：以累积乘积参数计算阶乘，展示尾递归形式。
 * 参数：n 为 0 到 20；accumulated_product 是此前各层已累积的乘积。
 * 返回值：最终阶乘；从初始乘积 1 调用时得到 n!。
 * 异常/错误：C 不使用异常；不检查参数，调用者须保证 0<=n<=20 且乘积可表示。
 * 副作用：无。
 * 复杂度：n 为参数值，时间 O(n)，递归栈辅助空间 O(n)；C 不保证编译器消除尾调用。
 * 用法示例：main 调用 factorial_tail_recursive(5U, 1ULL)，返回 120。
 */
static unsigned long long factorial_tail_recursive(
    unsigned int n,
    unsigned long long accumulated_product)
{
    if (n == 0U) {
        return accumulated_product;
    }

    return factorial_tail_recursive(
        n - 1U,
        accumulated_product * n);
}

/*
 * 用途：用循环计算非负整数的阶乘，作为尾递归版本的对照。
 * 参数：n 为 0 到 20。
 * 返回值：n!；此范围内结果可由 unsigned long long 表示。
 * 异常/错误：C 不使用异常；不检查参数，调用者须遵守范围前提。
 * 副作用：无。
 * 复杂度：n 为参数值，时间 O(n)，辅助空间 O(1)。
 * 用法示例：main 调用 factorial_iterative(5U)，返回 120。
 */
static unsigned long long factorial_iterative(unsigned int n)
{
    unsigned long long result = 1ULL;

    for (unsigned int factor = 2U; factor <= n; ++factor) {
        result *= factor;
    }

    return result;
}

/*
 * 用途：比较两种数字输出顺序，并运行尾递归和循环阶乘。
 * 参数：无；void 表示不接收命令行参数，数据由 main 固定提供。
 * 返回值：成功完成示例时返回 0。
 * 异常/错误：C 不使用异常；固定输入满足函数前提，标准输出错误未检查。
 * 副作用：向标准输出打印数字和阶乘结果。
 * 复杂度：d 为 UINT_MAX 的十进制位数，时间 O(d)，递归辅助空间 O(d)。
 * 用法示例：直接启动程序；观察 0、1203、UINT_MAX 的数字顺序及 5! 的两种结果。
 */
int main(void)
{
    puts("递归输出 0：");
    print_digits_recursive(0U);
    putchar('\n');

    puts("循环输出 0：");
    print_digits_iterative(0U);
    putchar('\n');

    puts("递归输出 1203：");
    print_digits_recursive(1203U);
    putchar('\n');

    puts("循环输出 1203：");
    print_digits_iterative(1203U);
    putchar('\n');

    printf("递归输出 UINT_MAX（当前实现的最大 unsigned int）：\n");
    print_digits_recursive(UINT_MAX);
    putchar('\n');

    printf("循环输出 UINT_MAX（当前实现的最大 unsigned int）：\n");
    print_digits_iterative(UINT_MAX);
    putchar('\n');

    printf("尾递归 5! = %llu\n",
           factorial_tail_recursive(5U, 1ULL));
    printf("循环 5! = %llu\n", factorial_iterative(5U));

    return 0;
}
