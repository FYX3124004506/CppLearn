/*
 * 演示整数算术、复合赋值、自增自减和安全的类型转换。
 * 输入：无。使用 C11 编译后运行。
 */
#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <limits.h>
#include <stddef.h>
#include <stdio.h>

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

    unsigned int mixed_signedness = -1 + 1U;
    unsigned int wrapped = UINT_MAX;
    wrapped += 1U;
    printf("混合有符号/无符号运算的结果=%u\n", mixed_signedness);
    printf("无符号 UINT_MAX 加一按模回绕：%u\n", wrapped);

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
    int counter = 3;
    int postfix_value = counter++;
    int prefix_value = ++counter;
    printf("连续赋值：%d、%d、%d\n", chain_a, chain_b, chain_c);
    printf("后置自增旧值=%d，前置自增结果=%d\n", postfix_value, prefix_value);
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