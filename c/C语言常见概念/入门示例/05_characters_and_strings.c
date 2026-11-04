/*
 * 文件用途：展示字符值、十进制数字字符转换、C 字符串结尾、NULL 指针、strlen 和 sizeof。
 * 输入：无；字符和数组内容由程序固定。
 * 输出：显示字符、转换结果、字符串长度及数组字节数。
 * 标准：C99 或更新版本；字符代码数值的说明以 ASCII 兼容执行字符集为条件。
 */
#include <stddef.h> /* 声明 size_t 和 NULL，供长度变量及空指针比较使用。 */
#include <stdio.h> /* 声明 printf 和 puts，供格式化输出及提示使用。 */
#include <string.h> /* 声明 strlen，供字符串长度示例使用。 */

/*
 * 用途：比较字符与字符串数据，并观察零字符对字符串函数的影响。
 * 参数：无；所有演示数据都在函数内固定。
 * 返回值：int；返回 0 表示程序正常结束。
 * 错误处理：不传递无效指针给字符串函数；NULL 指针只作比较，从不交给 %s 或 strlen。
 * 副作用：向标准输出写入字符及长度结果。
 */
int main(void) {
    char letter = 'A'; /* 声明 char 字符对象并保存基本执行字符 A。 */
    int execution_code = 'A'; /* 普通字符常量类型是 int；数值只在 ASCII 兼容执行字符集中为 65。 */
    char digit = '5'; /* 保存一个十进制数字字符，供字符到整数值转换使用。 */
    int digit_value = digit - '0'; /* 标准保证数字字符连续排列；相减得到整数 5，而不是字符编码表查询。 */
    char word[] = "hello"; /* 创建可修改字符数组；字符串字面量初始化后自动附加结尾零字符。 */
    char embedded[] = {'A', '\0', 'B', '\0'}; /* 逐个初始化四个元素；第一个零字符结束字符串，后面的 B 仍属于数组。 */
    char empty_text[] = ""; /* 空字符串仍有一个结尾零字符，因此这是有效的零结尾字符串。 */
    const char *missing_text = NULL; /* 创建不指向字符对象的空指针；它与字符串结束用的零字符不同。 */
    size_t word_length = strlen(word); /* strlen 返回 size_t，只计算第一个零字符之前的字符数。 */
    size_t word_bytes = sizeof word; /* word 仍是数组，sizeof 计算全部六个 char 元素占用的字节数。 */
    printf("character = %c\n", (int)letter); /* %c 要求 int 实参；显式转换后显示字符对象的值。 */
    printf("execution value of A = %d\n", execution_code); /* %d 显示执行字符集中的数值；ASCII 兼容环境中是 65。 */
    printf("digit = %c, value = %d\n", (int)digit, digit_value); /* 分别显示原字符和减去字符 0 得到的整数。 */
    printf("word = %s\n", word); /* %s 读取有效零结尾字符串，因此显示 hello。 */
    printf("strlen(word) = %d\n", (int)word_length); /* 固定长度为 5，可安全转成 int；%d 匹配 int。一般 size_t 可用 C99 的 %zu。 */
    printf("sizeof(word) = %d\n", (int)word_bytes); /* 固定数组大小为 6，可安全转成 int；结果包含结尾零字符。一般 size_t 可用 C99 的 %zu。 */
    printf("embedded as string = %s\n", embedded); /* %s 在第一个零字符处停止，所以只显示 A。 */
    printf("strlen(embedded) = %d\n", (int)strlen(embedded)); /* 固定结果为 1，可安全转成 int；strlen 不会读取零字符之后的 B。 */
    printf("sizeof(embedded) = %d\n", (int)sizeof embedded); /* 固定数组大小为 4，可安全转成 int；sizeof 仍看到全部元素。 */
    printf("empty string length = %d\n", (int)strlen(empty_text)); /* 固定长度为 0，可安全转成 int；空字符串仍有可读的结尾零字符。 */
    if (missing_text == NULL) { /* 比较指针是否为空；不把空指针当作字符数组或字符串。 */
        puts("string pointer is null"); /* 只有指针为空时输出说明；puts 自动换行。 */
    }
    return 0; /* 结束 main 并报告正常结束。 */
}

/* 示例输入：无；字符和数组内容固定。
 * 预期输出（在 ASCII 兼容执行字符集中）：
 * character = A
 * execution value of A = 65
 * digit = 5, value = 5
 * word = hello
 * strlen(word) = 5
 * sizeof(word) = 6
 * embedded as string = A
 * strlen(embedded) = 1
 * sizeof(embedded) = 4
 * empty string length = 0
 * string pointer is null
 * 关键结果：零字符终止字符串；NULL 表示空指针；strlen 测字符串长度，sizeof 测整个数组大小。
 */