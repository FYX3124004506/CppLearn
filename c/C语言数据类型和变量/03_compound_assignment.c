/*
 * 演示整数算术、复合赋值、自增自减和安全的类型转换。
 * 输入：无。使用 C11 编译后运行。
 */
/* 输出：算术与转换结果、边界规避、复合赋值、自增自减和按位运算结果。 */
#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <limits.h>
#include <stddef.h>
#include <stdio.h>

/*
 * 用途：演示整数/浮点算术、转换、复合赋值和自增自减，并避开危险边界操作。
 * 参数：无。
 * 返回值：0 表示示例正常结束。
 * 异常/错误：C 无异常机制；除数和移位边界由固定值与条件检查规避，标准输出错误未检查。
 * 副作用：向 stdout 输出结果；仅修改本地示例变量。
 * 复杂度：时间 O(1)，辅助空间 O(1)；算术操作和演示语句数量固定。
 * 示例：运行本文件进入 main(void)，观察 denominator 检查及 UINT_MAX 回绕示例。
 */
int main(void)
{
    puts("-- arithmetic, conversions, and assignment --");
    int sum = 9 + 4;
    int difference = 9 - 4;
    int product = 3 * 4;
    int integer_quotient = 6 / 4;
    int negative_quotient = -6 / 4;
    double floating_quotient = 6 / 4.0;
    int remainder = -13 % 5;
    double decimal = -3.14;
    int whole = (int)decimal;
    printf("算术：9+4=%d，9-4=%d，3*4=%d\n", sum, difference, product);
    printf("除法：6/4=%d，-6/4=%d，6/4.0=%.1f；-13%%5=%d\n",
           integer_quotient, negative_quotient, floating_quotient, remainder);
    printf("安全的浮点转整数：%.2f -> %d\n", decimal, whole);

    /* 构造一个大于 INT_MAX 的值来展示转换边界；不执行结果不可表示的有符号转换。 */
    unsigned long long outside_int_range = (unsigned long long)INT_MAX + 1ULL;
    printf("有符号转换边界：%llu > INT_MAX=%d，超范围转换已跳过。\n",
           outside_int_range, INT_MAX);

    /* -1 先转换为 unsigned int 的最大值，再加 1U 按模回绕为 0。 */
    unsigned int mixed_signedness = -1 + 1U;
    unsigned int wrapped = UINT_MAX;
    wrapped += 1U;
    printf("混合有符号/无符号运算的结果=%u\n", mixed_signedness);
    printf("无符号 UINT_MAX 加一按模回绕：%u\n", wrapped);

    /* denominator 是示例除数，当前值为 0；先检查再除，避免除零未定义行为。 */
    int denominator = 0;
    if (denominator == 0) {
        puts("除数为零时跳过除法和取余，避免未定义行为。");
    } else {
        printf("%d / %d = %d\n", 20, denominator, 20 / denominator);
    }
    if (INT_MIN < -INT_MAX) {
        puts("本实现的 INT_MIN / -1 商无法表示；只检查边界，不执行该除法。");
    } else {
        puts("本实现的 int 范围可表示 INT_MIN / -1 的数学商。");
    }

    int chain_a = 0;
    int chain_b = 0;
    int chain_c = 0;
    chain_a = chain_b = chain_c = 10;
    /* counter 是自增演示计数，初值 3；后置与前置操作后依次变为 4、5。 */
    int counter = 3;
    int postfix_value = counter++;
    int prefix_value = ++counter;
    printf("连续赋值：%d、%d、%d\n", chain_a, chain_b, chain_c);
    printf("后置自增旧值=%d，前置自增结果=%d\n", postfix_value, prefix_value);
    /* value 反复承载复合赋值的中间结果；每段都在执行前显式设定初值。 */
    int value = 5;
    value += 3;
    printf("+= gives %d\n", value);
    value *= 2;
    printf("*= gives %d\n", value);

    value = 20;
    value -= 8;
    printf("-= gives %d\n", value);
    value /= 3;
    printf("/= gives %d\n", value);
    value %= 3;
    printf("%%= gives %d\n", value);

    value = 2;
    value <<= 2;
    printf("<<= gives %d\n", value);
    value >>= 1;
    printf(">>= gives %d\n", value);
    value &= 6;
    printf("&= gives %d\n", value);
    value ^= 1;
    printf("^= gives %d\n", value);
    value |= 8;
    printf("|= gives %d\n", value);

    int items[] = {1, 2};
    size_t index = 0;
    items[index++] += 5; /* 复合赋值左侧只求值一次。 */
    printf("left side evaluated once: index=%zu, updated item=%d\n", index, items[0]);
    /* 移位数值均在可表示范围内；程序不执行可能产生未定义行为的操作。 */
    return 0;
}