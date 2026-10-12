/*
 * 文件用途：演示标准库函数与自定义函数、原型、返回类型转换、结构体返回和 void 函数提前返回。
 * 输入：无；全部使用固定数据，并安全处理一个空指针输入。
 * 输出：加法与结构体汇总、参数和返回类型转换、字符串长度判断、空指针拒绝结果和问候语。
 * 独立运行：gcc -std=c11 -Wall -Wextra -Wpedantic 函数基础.c -o 函数基础.exe
 */
#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <stddef.h>
#include <stdio.h>
#include <string.h>

/* 结构体按值返回两个相关整数，展示函数可以返回对象类型。 */
typedef struct {
    int sum;
    int product;
} OperationSummary;

/* 函数原型先声明返回类型和参数类型，定义在后面提供函数体。 */
int add(int left, int right);
OperationSummary summarize(int left, int right);
void print_summary(OperationSummary summary);
int convert_return(double value);
int text_matches_length(const char *text, size_t expected_length);
void print_if_present(const char *text);

/*
 * 在 C11 中，声明里的空参数列表不提供参数类型信息；C23 中该写法表示无参数。
 * 此声明仅用于展示 C11 旧式接口，不会被调用；明确无参数时使用 (void)。
 */
int unspecified_arguments();

/* C 函数定义处于文件作用域；函数体内可以调用其他函数，但不能再定义函数。 */
/*
 * 用途：程序入口，按顺序调用标准库函数和自定义函数。
 * 参数：无；void 表示不接收启动参数。
 * 返回值：int；在宿主 C 环境中，执行到 main 末尾等价于返回 0。
 * 异常/错误：C 不使用异常；固定输入满足函数前提；标准输出错误未统一检查。
 * 副作用：向标准输出写入多行结果。
 * 复杂度：时间 O(n)，n 为固定字符串的长度；辅助空间 O(1)。
 * 示例：运行生成的程序时，C 运行环境调用 main(void)，无需额外输入。
 */
int main(void)
{
    OperationSummary summary = summarize(6, 7);
    int converted_argument = add(1.9, 2);
    int converted = convert_return(3.75);
    int has_expected_length = text_matches_length("C functions", 11U);
    int rejects_null_text = !text_matches_length(NULL, 0U);

    printf("add(6, 7) = %d\n", add(6, 7));
    print_summary(summary);
    printf("double argument 1.9 converted to int: %d\n", converted_argument);
    printf("convert_return(3.75) = %d\n", converted);
    printf("strlen length matches 11 bytes: %s\n",
           has_expected_length ? "yes" : "no");
    printf("null text rejected: %s\n",
           rejects_null_text ? "yes" : "no");
    print_if_present(NULL);
    print_if_present("hello from a custom function");

    /* 宿主 C 环境中，main 执行到函数末尾等价于返回 0。 */
}

/*
 * 用途：返回两个输入整数的和。
 * 参数：left、right 为参与相加的 int 值；要求数学上的和可由 int 表示。
 * 返回值：两数之和。
 * 异常/错误：C 不使用异常；没有额外错误码，调用者须遵守可表示范围前提。
 * 副作用：无。
 * 复杂度：时间 O(1)，辅助空间 O(1)；只进行一次整数加法。
 * 示例：main 调用 add(6, 7) 返回 13；add(1.9, 2) 的第一个实参转换为 int 1，结果为 3。
 */
int add(int left, int right)
{
    return left + right;
}

/*
 * 用途：根据两个整数构造和与积组成的结果对象。
 * 参数：left、right 为输入整数；要求它们的和与积都可由 int 表示。
 * 返回值：包含和与积的 OperationSummary 结构体值。
 * 异常/错误：C 不使用异常；不检查算术溢出，调用者须满足参数范围前提。
 * 副作用：无。
 * 复杂度：时间 O(1)，辅助空间 O(1)；结果结构体固定包含两个 int。
 * 示例：main 调用 summarize(6, 7)，得到 sum 为 13、product 为 42 的结构体。
 */
OperationSummary summarize(int left, int right)
{
    OperationSummary result;

    result.sum = add(left, right);
    result.product = left * right;
    return result;
}

/*
 * 用途：输出结构体中保存的和与积。
 * 参数：summary 是按值传入的汇总对象，函数只读取其中两个字段。
 * 返回值：无。
 * 异常/错误：C 不使用异常；printf 的输出错误不检查。
 * 副作用：向标准输出写一行文本。
 * 复杂度：时间 O(1)，辅助空间 O(1)；输出字段数量固定。
 * 示例：main 将 summarize(6, 7) 的结果传给 print_summary。
 */
void print_summary(OperationSummary summary)
{
    printf("summary: sum=%d, product=%d\n",
           summary.sum, summary.product);
}

/*
 * 用途：把 double 返回表达式转换为 int，演示返回值类型转换。
 * 参数：value 是有限 double，且位于 int 可表示范围内。
 * 返回值：向零截去小数部分后的 int 值。
 * 异常/错误：C 不使用异常；不校验输入范围，超出 int 可表示范围不在本函数契约内。
 * 副作用：无。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 示例：main 调用 convert_return(3.75)，返回 3。
 */
int convert_return(double value)
{
    return value;
}

/*
 * 用途：调用标准库 strlen，判断字符串是否具有指定字节长度。
 * 参数：text 为以空字符结尾的字符串指针，可为 NULL；expected_length 为期望字节数。
 * 返回值：长度匹配返回 1；空指针或长度不匹配返回 0。
 * 异常/错误：C 不使用异常；空指针会被拒绝，非空参数必须指向有效且以空字符结尾的字符数组。
 * 副作用：无。
 * 复杂度：时间 O(n)，n 为从首字符到空字符前的字节数；辅助空间 O(1)。
 * 示例：main 分别传入有效字符串和 NULL；前者长度匹配返回 1，后者被拒绝并返回 0。
 */
int text_matches_length(const char *text, size_t expected_length)
{
    if (text == NULL) {
        return 0;
    }

    return strlen(text) == expected_length;
}

/*
 * 用途：仅在输入指针非空时输出一行字符串，展示 void 函数的提前 return。
 * 参数：text 可为空；非空时必须指向有效且以空字符结尾的字符串。
 * 返回值：无。
 * 异常/错误：C 不使用异常；空指针触发提前返回；puts 的输出错误不检查。
 * 副作用：text 非空时向标准输出写入一行文本。
 * 复杂度：时间 O(n)，n 为字符串长度；辅助空间 O(1)。
 * 示例：main 先调用 print_if_present(NULL)（无输出），再调用 print_if_present("hello from a custom function")。
 */
void print_if_present(const char *text)
{
    if (text == NULL) {
        return;
    }

    puts(text);
}
