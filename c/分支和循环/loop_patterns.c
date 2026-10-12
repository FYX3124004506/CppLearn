/*
 * 用途：展示 while、for、do while、循环控制、嵌套试除与函数内跳转。
 * 示例数据：固定计数、十进制整数、候选素数范围和嵌套循环坐标。
 * 输出：计数序列、反向数字、位数、素数及 break/continue/goto 的控制结果。
 * 编译运行：gcc -std=c11 -Wall -Wextra -Wpedantic loop_patterns.c -o loop_patterns
 */
/* 输入：计数、整数样例及循环边界均在程序内固定，不读取 stdin。 */

#include <limits.h>
#include <stdio.h>

/*
 * 用途：计算 int 值十进制表示的位数，不把负号计入位数。
 * 参数：value 为任意 int，包括 0 和 INT_MIN。
 * 返回值：十进制数字位数；0 返回 1。
 * 异常/错误：C 无异常机制；所有 int 值均可处理，无错误返回路径。
 * 副作用：无。
 * 复杂度：时间 O(D)，D 为十进制位数；辅助空间 O(1)。
 * 示例：show_do_while_examples 对 0、-1200 和 INT_MIN 调用 count_decimal_digits。
 */
static int count_decimal_digits(int value)
{
    /*
     * 负数转为 unsigned 后按 UINT_MAX + 1 取模，再用无符号减法取得大小，
     * 不依赖 long long 是否比 int 多出一个可表示的正值，也不会对 INT_MIN 有符号取反。
     * do while 让 0 至少计为一位。
     */
    /* magnitude 是绝对值大小的无符号表示；单位为数值，不含负号，范围可覆盖 INT_MIN。 */
    unsigned int magnitude = value < 0
        ? 0U - (unsigned int)value
        : (unsigned int)value;
    int digits = 0;

    do {
        ++digits;
        magnitude /= 10U;
    } while (magnitude > 0U);

    return digits;
}

/*
 * 用途：判断 int 是否为素数，仅对大于等于 2 的候选数试除。
 * 参数：number 为待判断整数。
 * 返回值：1 表示素数，0 表示非素数。
 * 异常/错误：C 无异常机制；负数、0 和 1 直接返回 0，不存在错误返回值。
 * 副作用：无。
 * 复杂度：时间 O(√n)，n 为正候选数；辅助空间 O(1)。
 * 示例：show_nested_prime_search 对 100..200 的每个 candidate 调用 is_prime(candidate)。
 */
static int is_prime(int number)
{
    if (number < 2) {
        return 0;
    }

    /*
     * 只需检查到平方根。除法写法避免 divisor * divisor 溢出。
     */
    for (int divisor = 2; divisor <= number / divisor; ++divisor) {
        if (number % divisor == 0) {
            return 0;
        }
    }

    return 1;
}

/*
 * 用途：演示 while 的计数、数字逆序和条件为假时零次执行。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；循环值固定且除数为 10，示例无输入错误路径。
 * 副作用：向 stdout 输出计数与逆序结果。
 * 复杂度：时间 O(N + D)，N 为计数上界（当前 10），D 为 1200 的十进制位数；辅助空间 O(1)。
 * 示例：main 直接调用 show_while_examples()。
 */
static void show_while_examples(void)
{
    puts("=== while ===");

    int number = 1;
    while (number <= 10) {
        printf("%d ", number);
        ++number;
    }
    putchar('\n');

    /* 循环体按末位到首位输出；原数 1200 的结果保留了两个前导零。 */
    /* positive_number 是待拆分的非负十进制数；当前值 1200，循环逐次除以 10。 */
    int positive_number = 1200;
    printf("1200 的逆序数字：");
    while (positive_number > 0) {
        printf("%d", positive_number % 10);
        positive_number /= 10;
    }
    putchar('\n');

    /* while 在循环体执行前判断，初始条件为假时可能零次执行。 */
    int zero_iterations = 0;
    int false_condition = 0;
    while (false_condition) {
        ++zero_iterations;
    }
    printf("初始条件为假时，while 执行次数：%d\n", zero_iterations);
}

/*
 * 用途：演示 for 的零次执行、累加、作用域、自增和 break 终止。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；循环范围固定且累加值可由 int 表示。
 * 副作用：向 stdout 输出固定循环结果。
 * 复杂度：时间 O(B)，B 为倍数循环的上界（当前 100）；辅助空间 O(1)。
 * 示例：main 直接调用 show_for_examples()。
 */
static void show_for_examples(void)
{
    puts("\n=== for ===");

    /* for 在循环体执行前判断，与 while 一样可能零次执行。 */
    int zero_iterations = 0;
    for (int candidate = 1; candidate < 1; ++candidate) {
        ++zero_iterations;
    }
    printf("for 初始条件为假时的执行次数：%d\n", zero_iterations);

    /* sum 累计 1..100 内 3 的倍数；单位为整数和，最终值为 1683。 */
    int sum = 0;
    for (int multiple = 3; multiple <= 100; multiple += 3) {
        sum += multiple;
    }
    printf("1..100 中 3 的倍数之和：%d\n", sum);

    int scope_sum = 0;
    for (int index = 1; index <= 3; ++index) {
        scope_sum += index;
    }

    /* for 初始化位置声明的 index 到循环结束时离开作用域，可在此处重新声明。 */
    int index = 99;
    printf("循环内累计=%d，循环后重新声明的 index=%d\n", scope_sum, index);

    int value = 4;
    int old_value = value++;
    int new_value = ++value;
    printf("后置结果=%d，前置结果=%d，最终值=%d\n",
           old_value, new_value, value);

    int bounded_runs = 0;
    for (;;) {
        ++bounded_runs;
        if (bounded_runs == 3) {
            break;
        }
    }
    printf("省略 for 条件后由 break 结束，循环次数：%d\n", bounded_runs);
}

/*
 * 用途：演示 do while 至少执行一次，并测试包含 INT_MIN 的十进制位数。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；样例均为 int 可表示值，count_decimal_digits 覆盖全部 int 输入。
 * 副作用：向 stdout 输出计数序列和位数结果。
 * 复杂度：时间 O(ND)，N 为样例数（当前 3），D 为最大十进制位数；辅助空间 O(1)。
 * 示例：main 直接调用 show_do_while_examples()，其中对 samples[index] 调用 count_decimal_digits。
 */
static void show_do_while_examples(void)
{
    puts("\n=== do while ===");

    int number = 1;
    do {
        printf("%d ", number);
        ++number;
    } while (number <= 10);
    putchar('\n');

    int runs = 0;
    do {
        ++runs;
    } while (0);
    printf("条件一开始为假，do while 仍执行 %d 次\n", runs);

    /* samples 固定包含零、普通负数和 int 最小值；index 范围为 0..2。 */
    const int samples[] = {0, -1200, INT_MIN};
    for (int index = 0; index < 3; ++index) {
        printf("%d 的十进制位数：%d\n",
               samples[index], count_decimal_digits(samples[index]));
    }
}

/*
 * 用途：展示 for/while/do while 中 continue、break 的作用位置及嵌套循环层级。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；循环变量范围固定，while 在 continue 前先更新控制变量。
 * 副作用：向 stdout 输出循环访问序列。
 * 复杂度：时间 O(1)，循环上界固定为 10、5 及 3×4；辅助空间 O(1)。
 * 示例：main 直接调用 show_break_and_continue()。
 */
static void show_break_and_continue(void)
{
    puts("\n=== break 与 continue ===");

    printf("for 跳过 3 并在 8 处结束：");
    for (int number = 1; number <= 10; ++number) {
        if (number == 3) {
            continue;
        }
        if (number == 8) {
            break;
        }
        printf("%d ", number);
    }
    putchar('\n');

    printf("while 中先更新再 continue：");
    /* number 是 while 控制变量，先递增再可能 continue，保证取值从 0 推进到 5。 */
    int number = 0;
    while (number < 5) {
        int current = number;
        /* 先推进控制变量，再可能 continue，避免跳过更新后停在同一值。 */
        ++number;
        if (current == 2) {
            continue;
        }
        printf("%d ", current);
    }
    putchar('\n');

    int attempts = 0;
    do {
        ++attempts;
        if (attempts < 3) {
            continue;
        }
        printf("do while 的 continue 仍会检查条件，最终次数：%d\n", attempts);
    } while (attempts < 3);

    /* 此 break 只退出 column 循环；row 仍继续，因此每行都会输出两列坐标。 */
    printf("内层 break 不会结束外层循环：");
    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 4; ++column) {
            if (column == 2) {
                break;
            }
            printf("(%d,%d) ", row, column);
        }
    }
    putchar('\n');
}

/*
 * 用途：扫描 100..200 的候选整数，打印素数并统计个数。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；候选范围固定且 is_prime 对所有 int 有定义。
 * 副作用：向 stdout 输出素数序列与总数。
 * 复杂度：时间 O(R√H)，R 为候选数个数（当前 101），H 为最大候选值（当前 200）；辅助空间 O(1)。
 * 示例：main 直接调用 show_nested_prime_search()，函数内逐个调用 is_prime(candidate)。
 */
static void show_nested_prime_search(void)
{
    puts("\n=== 100 到 200 的素数 ===");

    /* count 的单位为个素数；candidate 的范围为闭区间 100..200。 */
    int count = 0;
    for (int candidate = 100; candidate <= 200; ++candidate) {
        if (is_prime(candidate)) {
            printf("%d ", candidate);
            ++count;
        }
    }
    printf("\n共找到 %d 个。\n", count);
}

/*
 * 用途：演示 goto 跳出嵌套循环、标签作用域及条件支持下的 VLA 作用域跳出。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；只执行合法的跳出 VLA 作用域路径，不跳入其作用域。
 * 副作用：向 stdout 输出访问坐标与 VLA 支持信息；VLA 分支使用固定长度 2。
 * 复杂度：时间 O(1)，辅助空间 O(1)（VLA 长度固定为 2）；嵌套循环界限均为 3。
 * 示例：main 直接调用 show_goto_examples()。
 */
static void show_goto_examples(void)
{
    puts("\n=== goto 与作用域 ===");

    for (int row = 0; row < 3; ++row) {
        for (int column = 0; column < 3; ++column) {
            printf("访问 (%d,%d)\n", row, column);
            if (row == 1 && column == 1) {
                goto finished_search;
            }
        }
    }

finished_search:
    puts("goto 从两层循环跳到同一函数中的标签。");

#if !defined(__STDC_NO_VLA__) || (__STDC_NO_VLA__ == 0)
    /* length 的单位为 int 元素个数，当前为 2；VLA 仅在实现支持时声明。 */
    int length = 2;
    {
        int values[length];
        values[0] = 4;
        values[1] = 5;
        printf("支持 VLA 的实现中，数组值为 %d 和 %d。\n",
               values[0], values[1]);
        goto outside_vla_scope;
    }

outside_vla_scope:
    puts("合法地从 VLA 作用域跳出；程序没有尝试非法跳入该作用域。");
#else
    puts("当前 C11 实现不支持可选的 VLA 演示。");
#endif
}

/*
 * 用途：依次运行本文件的循环、素数和跳转示例。
 * 参数：无。
 * 返回值：0 表示所有演示函数正常返回。
 * 异常/错误：C 无异常机制；被调函数无错误返回值，标准输出错误未检查。
 * 副作用：按固定顺序调用六个演示函数并向 stdout 输出结果。
 * 复杂度：时间 O(1)，辅助空间 O(1)；所有循环规模由源码中的常量界限固定。
 * 示例：运行本文件进入 main(void)，依次调用 show_while_examples() 至 show_goto_examples()。
 */
int main(void)
{
    show_while_examples();
    show_for_examples();
    show_do_while_examples();
    show_break_and_continue();
    show_nested_prime_search();
    show_goto_examples();
    return 0;
}
