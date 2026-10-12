/*
 * 用途：观察条件判断、逻辑求值、条件与逗号运算符及 switch 分支。
 * 示例数据：程序内固定的数字、年份、选择值和年龄。
 * 输出：各表达式的结果、被选中的分支以及短路时的副作用计数。
 * 编译运行：gcc -std=c11 -Wall -Wextra -Wpedantic branch_operators.c -o branch_operators
 */
/* 输入：示例数字、年份、选项和年龄均在程序内固定，不读取 stdin。 */

#include <stddef.h>
#include <stdio.h>

/*
 * 用途：记录一次逻辑右侧的执行，并返回真值以展示短路求值。
 * 参数：label 为非空 C 字符串；count 为有效的 int 计数器指针。
 * 返回值：固定返回 1。
 * 异常/错误：C 无异常机制；不检查空指针或上溢，调用前须保证 label、count 有效且递增不使 int 上溢。
 * 副作用：递增 *count 并向 stdout 输出 label。
 * 复杂度：时间 O(L)，L 为 label 字符数；辅助空间 O(1)。
 * 示例：show_short_circuit 在逻辑右侧条件成立时调用 record_side_effect("逻辑或左侧为假", &side_effect_count)。
 */
static int record_side_effect(const char *label, int *count)
{
    ++*count;
    printf("  右侧实际执行：%s\n", label);
    return 1;
}

/*
 * 用途：显示条件运算符选中的分支并传回该分支值。
 * 参数：label 为非空 C 字符串；value 为要返回的 int 值。
 * 返回值：原样返回 value。
 * 异常/错误：C 无异常机制；不检查空指针，调用时需提供有效 label，标准输出错误未检查。
 * 副作用：向 stdout 输出 label。
 * 复杂度：时间 O(L)，L 为 label 字符数；辅助空间 O(1)。
 * 示例：show_conditional_operator 在 choose_left 为真时调用 report_choice("左侧", 5)。
 */
static int report_choice(const char *label, int value)
{
    printf("  被选中的分支：%s\n", label);
    return value;
}

/*
 * 用途：演示 if 对整数、浮点值、指针的真假判断及嵌套分支配对。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；本例用固定有效对象，无显式错误路径，输出错误未检查。
 * 副作用：向 stdout 输出判断结果。
 * 复杂度：时间 O(n)，n 为 numbers 数组元素数（当前 3）；辅助空间 O(1)。
 * 示例：main 直接调用 show_if_and_conditions()。
 */
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

    /* numbers 的固定样例范围为正数、负数和零；index 按元素下标 0..2 递增。 */
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

/*
 * 用途：演示比较结果、区间判断、短路逻辑表达式和闰年条件。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；示例使用固定 int 值和非零除数，无错误输入路径。
 * 副作用：向 stdout 输出比较和逻辑结果。
 * 复杂度：时间 O(n)，n 为 years 数组元素数（当前 4）；辅助空间 O(1)。
 * 示例：main 直接调用 show_comparisons_and_logic()。
 */
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

    /* age 的单位为年；本例 50 超出所演示的 18..40 区间。 */
    int age = 50;
    int first_comparison = 18 <= age;
    /* first_comparison 只有 0/1；再与 40 比较不是年龄区间，应使用两个条件和 &&。 */
    int chained_as_written = first_comparison <= 40;
    int correct_range = age >= 18 && age <= 40;
    printf("年龄 %d：把关系比较结果再与 40 比较会得到 %d；区间判断为 %d\n",
           age, chained_as_written, correct_range);

    /* years 保存公历年份样例；是否闰年按 400/100/4 的规则组合判断。 */
    const int years[] = {2024, 1900, 2000, 2023};
    for (size_t index = 0; index < sizeof years / sizeof years[0]; ++index) {
        int year = years[index];
        /* 400 年规则优先；整除 100 的年份须排除，除非也整除 400。 */
        int is_leap_year =
            year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
        printf("%d 年是否闰年：%d；逻辑取反：%d\n",
               year, is_leap_year, !is_leap_year);
    }
}

/*
 * 用途：用可观察计数演示 &&、|| 的短路规则和空指针保护。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；optional_value 先与 NULL 比较，短路保护后续解引用。
 * 副作用：调用 record_side_effect 时递增局部计数器并输出提示；另输出汇总结果。
 * 复杂度：时间 O(1)，辅助空间 O(1)；表达式和副作用调用数固定。
 * 示例：main 直接调用 show_short_circuit()；该函数内逻辑表达式调用 record_side_effect。
 */
static void show_short_circuit(void)
{
    puts("\n=== 短路求值 ===");

    /* side_effect_count 统计右侧函数实际执行次数；本例末值为 2，计数单位为次。 */
    int side_effect_count = 0;
    int and_gate = 0;
    int or_gate = 1;

    /* && 左侧为假或 || 左侧为真时，右侧函数不会执行，计数保持不变。 */
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

/*
 * 用途：演示条件运算符只求值被选分支，并展示算术分支的共同类型转换。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；固定分支值均可表示，输出错误未检查。
 * 副作用：通过 report_choice 输出所选分支，再向 stdout 输出转换结果。
 * 复杂度：时间 O(1)，辅助空间 O(1)；只选择并输出固定分支值。
 * 示例：main 直接调用 show_conditional_operator()，其中条件表达式调用 report_choice。
 */
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

/*
 * 用途：演示逗号运算符按顺序求值，并区分实参分隔逗号。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；固定 int 算术均在可表示范围内，输出错误未检查。
 * 副作用：修改局部 first、second 并向 stdout 输出结果。
 * 复杂度：时间 O(1)，辅助空间 O(1)；逗号表达式包含固定三个子表达式。
 * 示例：main 直接调用 show_comma_operator()。
 */
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

/*
 * 用途：演示 switch 的 case/default、break、贯穿执行和负余数归一化。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；余数示例除数固定为 3，避免除零，输出错误未检查。
 * 副作用：向 stdout 输出匹配结果。
 * 复杂度：时间 O(1)，辅助空间 O(1)；各 switch 的分支数固定。
 * 示例：main 直接调用 show_switch()；函数内 option=1 演示有意贯穿到 case 2。
 */
static void show_switch(void)
{
    puts("\n=== switch 与余数 ===");

    int number = 4;
    /* 正数 number=4 时 remainder 位于 0..2；负数示例另行归一化到该范围。 */
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
    /* C 的负余数可为负；加上正除数后再取余，将结果归一到 0..2。 */
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

/*
 * 用途：按主题顺序运行本文件中的条件判断和运算符演示。
 * 参数：无。
 * 返回值：0 表示所有演示函数正常返回。
 * 异常/错误：C 无异常机制；被调函数无错误返回值，标准输出错误未检查。
 * 副作用：依次调用六个演示函数并向 stdout 输出内容。
 * 复杂度：时间 O(1)，辅助空间 O(1)；调用数量和各样例规模固定。
 * 示例：运行本文件进入 main(void)，依次调用 show_if_and_conditions() 至 show_switch()。
 */
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
