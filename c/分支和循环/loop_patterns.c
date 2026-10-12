/*
 * 用途：展示 while、for、do while、循环控制、嵌套试除与函数内跳转。
 * 示例数据：固定计数、十进制整数、候选素数范围和嵌套循环坐标。
 * 输出：计数序列、反向数字、位数、素数及 break/continue/goto 的控制结果。
 * 编译运行：gcc -std=c11 -Wall -Wextra -Wpedantic loop_patterns.c -o loop_patterns
 */

#include <limits.h>
#include <stdio.h>

static int count_decimal_digits(int value)
{
    /*
     * 负数转为 unsigned 后按 UINT_MAX + 1 取模，再用无符号减法取得大小，
     * 不依赖 long long 是否比 int 多出一个可表示的正值，也不会对 INT_MIN 有符号取反。
     * do while 让 0 至少计为一位。
     */
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

static void show_for_examples(void)
{
    puts("\n=== for ===");

    /* for 在循环体执行前判断，与 while 一样可能零次执行。 */
    int zero_iterations = 0;
    for (int candidate = 1; candidate < 1; ++candidate) {
        ++zero_iterations;
    }
    printf("for 初始条件为假时的执行次数：%d\n", zero_iterations);

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

    const int samples[] = {0, -1200, INT_MIN};
    for (int index = 0; index < 3; ++index) {
        printf("%d 的十进制位数：%d\n",
               samples[index], count_decimal_digits(samples[index]));
    }
}

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

static void show_nested_prime_search(void)
{
    puts("\n=== 100 到 200 的素数 ===");

    int count = 0;
    for (int candidate = 100; candidate <= 200; ++candidate) {
        if (is_prime(candidate)) {
            printf("%d ", candidate);
            ++count;
        }
    }
    printf("\n共找到 %d 个。\n", count);
}

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
