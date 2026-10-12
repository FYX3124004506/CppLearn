/*
 * 用途：观察条件判断、逻辑求值、条件与逗号运算符及 switch 分支。
 * 示例数据：程序内固定的数字、年份、选择值和年龄。
 * 输出：各表达式的结果、被选中的分支以及短路时的副作用计数。
 * 编译运行：gcc -std=c11 -Wall -Wextra -Wpedantic branch_operators.c -o branch_operators
 */

#include <stddef.h>
#include <stdio.h>

static int record_side_effect(const char *label, int *count)
{
    ++*count;
    printf("  右侧实际执行：%s\n", label);
    return 1;
}

static int report_choice(const char *label, int value)
{
    printf("  被选中的分支：%s\n", label);
    return value;
}

static void show_if_and_conditions(void)
{
    puts("=== if 与条件值 ===");

    int zero = 0;
    int nonzero = -3;
    double fraction = 0.25;
    const char *message = "非空字符串";

    printf("整数 0：");
    if (zero) {
        puts("真");
    } else {
        puts("假");
    }

    printf("非零整数：");
    if (nonzero) {
        puts("真");
    } else {
        puts("假");
    }

    printf("非零浮点数：");
    if (fraction) {
        puts("真");
    } else {
        puts("假");
    }

    printf("非空指针：");
    if (message) {
        puts("真");
    } else {
        puts("假");
    }

    const int numbers[] = {5, -4, 0};
    for (size_t index = 0; index < sizeof numbers / sizeof numbers[0]; ++index) {
        int number = numbers[index];
        printf("分类 %d：", number);

        if (number > 0) {
            printf("正数，");
            if (number % 2 == 0) {
                puts("偶数");
            } else {
                puts("奇数");
            }
        } else if (number < 0) {
            puts("负数");
        } else {
            puts("零");
        }
    }

    /*
     * 花括号让 else 的归属一眼可见：它属于最近的内层 if。
     * 即使去掉花括号，C 的语法规则也仍把 else 配给最近的未匹配 if。
     */
    int outer_condition = 1;
    int inner_condition = 0;
    if (outer_condition) {
        if (inner_condition) {
            puts("内层条件为真");
        } else {
            puts("else 配对内层 if");
        }
    }
}

static void show_comparisons_and_logic(void)
{
    puts("\n=== 比较、逻辑运算与闰年 ===");

    int larger = 7;
    int smaller = 3;
    printf("关系结果 > %d，< %d，>= %d，<= %d\n",
           larger > smaller,
           larger < smaller,
           larger >= smaller,
           larger <= smaller);
    printf("相等结果 == %d，不等结果 != %d\n",
           larger == smaller,
           larger != smaller);

    int assigned_value = 5;
    int equals_five = assigned_value == 5;
    printf("赋值后的值=%d；用 == 判断是否等于 5：%d\n",
           assigned_value, equals_five);

    int age = 50;
    int first_comparison = 18 <= age;
    int chained_as_written = first_comparison <= 40;
    int correct_range = age >= 18 && age <= 40;
    printf("年龄 %d：把关系比较结果再与 40 比较会得到 %d；区间判断为 %d\n",
           age, chained_as_written, correct_range);

    const int years[] = {2024, 1900, 2000, 2023};
    for (size_t index = 0; index < sizeof years / sizeof years[0]; ++index) {
        int year = years[index];
        int is_leap_year =
            year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
        printf("%d 年是否闰年：%d；逻辑取反：%d\n",
               year, is_leap_year, !is_leap_year);
    }
}

static void show_short_circuit(void)
{
    puts("\n=== 短路求值 ===");

    int side_effect_count = 0;
    int and_gate = 0;
    int or_gate = 1;

    int and_result = and_gate && record_side_effect("逻辑与", &side_effect_count);
    int or_result = or_gate || record_side_effect("逻辑或", &side_effect_count);
    printf("短路后：&& 结果 %d，|| 结果 %d，副作用次数 %d\n",
           and_result, or_result, side_effect_count);

    int false_or_gate = 0;
    int evaluated_or_result =
        false_or_gate || record_side_effect("逻辑或左侧为假", &side_effect_count);
    printf("|| 左侧为假：结果 %d，副作用次数 %d\n",
           evaluated_or_result, side_effect_count);

    int enabled_gate = 1;
    int evaluated_result =
        enabled_gate && record_side_effect("左侧为真时的逻辑与", &side_effect_count);
    printf("左侧为真：结果 %d，副作用次数 %d\n",
           evaluated_result, side_effect_count);

    const int *optional_value = NULL;
    int has_positive_value =
        optional_value != NULL && *optional_value > 0;
    printf("空指针先判断再访问：%d（0 表示无正值）\n",
           has_positive_value);
}

static void show_conditional_operator(void)
{
    puts("\n=== 条件运算符 ===");

    int choose_left = 1;
    int selected = choose_left
        ? report_choice("左侧", 5)
        : report_choice("右侧", 9);
    printf("选择结果：%d\n", selected);

    double common_arithmetic_type = choose_left ? 1 : 2.5;
    printf("算术分支按共同类型转换后为 %.1f\n", common_arithmetic_type);
}

static void show_comma_operator(void)
{
    puts("\n=== 逗号运算符 ===");

    int first = 1;
    int second = 5;
    int result = (first += 2, second *= 3, first + second);

    printf("从左到右执行后：first=%d，second=%d，最右侧结果=%d\n",
           first, second, result);
    printf("函数调用中的两个实参由逗号分隔：%d 和 %d\n", first, second);
}

static void show_switch(void)
{
    puts("\n=== switch 与余数 ===");

    int number = 4;
    int remainder = number % 3;
    printf("%d %% 3 的余数分类：", number);
    switch (remainder) {
        case 0:
            puts("0");
            break;
        case 1:
            puts("1");
            break;
        case 2:
            puts("2");
            break;
        default:
            puts("不在这三个非负余数中");
            break;
    }

    int option = 1;
    puts("有意贯穿到后续 case：");
    switch (option) {
        case 1:
            puts("匹配 case 1");
            /* fall through */
        case 2:
            puts("继续执行 case 2 的语句");
            break;
        default:
            puts("其他选项");
            break;
    }

    int unmatched = 7;
    switch (unmatched) {
        default:
            puts("default 放在前面也会处理未匹配值");
            break;
        case 0:
            puts("匹配值 0");
            break;
    }

    int negative = -4;
    int c_remainder = negative % 3;
    int normalized_remainder = (c_remainder + 3) % 3;
    printf("负数余数：%d %% 3 = %d；归一化到 0..2 后为 %d\n",
           negative, c_remainder, normalized_remainder);

    int unmatched_without_default = 7;
    int branch_runs = 0;
    /* 没有 default 且 case 不匹配时，switch 不执行分支，随后继续运行。 */
    switch (unmatched_without_default) {
        case 0:
            branch_runs = 1;
            break;
    }
    printf("没有 default 且没有匹配 case：分支执行次数 %d，switch 后继续运行\n",
           branch_runs);
}

int main(void)
{
    show_if_and_conditions();
    show_comparisons_and_logic();
    show_short_circuit();
    show_conditional_operator();
    show_comma_operator();
    show_switch();
    return 0;
}
