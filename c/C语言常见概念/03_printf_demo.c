/*
 * 演示 printf 转换说明、默认参数提升、字段宽度和精度。
 * 输入：无。使用 C11 编译后运行。
 */
/*
 * 文件用途：示范 printf 的整数、字符、浮点、字符串、指针及格式宽度/精度。
 * 输入：无；示例数据均在程序中固定给出。
 * 输出：多行格式化文本写到标准输出，最后一行以换行结束。
 */
#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <stddef.h>
#include <stdio.h>

/* 每个转换说明都要匹配经过默认参数提升后的实参类型；不匹配可能导致未定义行为。
 * printf 不会自动换行，只有格式字符串包含 \n 时才输出换行符。
 */

/*
 * 用途：按成组的示例值调用 printf，展示转换说明和格式控制。
 * 参数：无；所有值都在函数内初始化。
 * 返回值：int；返回 0 表示正常结束。
 * 异常/错误：C 不使用异常机制；格式与实参由源码配对；各次 printf 的写出错误没有检查，最后仍返回 0。
 * 副作用：多次向标准输出写入格式化文本；取得 age 地址仅用于输出，不修改对象。
 * 复杂度：时间 O(1)，辅助空间 O(1)；格式串、数组和调用次数均固定。
 * 示例：main 中实际调用 printf 输出 age 的整数格式、pi 的字段宽度/精度格式等；运行后可观察对齐和舍入。
 */
int main(void) {
    // %d 输出十进制整数。
/*
 * 关键变量：age 是以“岁”为单位的年龄示例，当前固定为 18；假设/待确认：population 等整数值没有提供来源，只用于格式演示，不代表真实人口数据。
 */
    int age = 18;
    long population = 123456L;
    unsigned long unsigned_population = 123456UL;
    long long large_population = 9876543210LL;
    unsigned long long unsigned_large_population = 9876543210ULL;
    printf("年龄：%d\n", age);
    printf("有符号十进制：%d / %i\n", age, age);
    printf("长整数：%ld/%lu，long long：%lld/%llu\n",
           population, unsigned_population, large_population, unsigned_large_population);

    // %c 输出字符，%d 输出执行字符集中的编码值。
    char ch = 'A';
    printf("字符：%c\n", ch);
    printf("字符编码值：%d\n", ch);

    // %f 输出 double；精度和字段宽度可以写在格式符中。
/*
 * 关键变量：pi、rate、third 分别演示 double、float 和 long double；它们是无单位的示例数值。
 */
    double pi = 3.1415926;
    float rate = 1.25f;
    long double third = 1.0L / 3.0L;
    printf("圆周率：%f\n", pi);
    printf("圆周率：%.2f\n", pi);
    printf("圆周率：%8.3f\n", pi);
    printf("float/double/long double：%.2f、%e、%g、%.4Lf\n",
           rate, pi, pi, third);

    // %s 输出以 \0 结尾的字符串。
/*
 * 关键变量：name 和 sample 都是自动以空字符结束的只读字符数组；age_size 的单位是字节，值为 sizeof age。
 */
    const char name[] = "张三";
    const char sample[] = "hello world";
    size_t age_size = sizeof age;
    printf("姓名：%s\n", name);

    // %o、%x 和 %X 分别输出八进制、小写十六进制和大写十六进制。
/*
 * 关键变量：number 是无符号整数示例，当前值 255；可表示范围取决于 unsigned int 的实现范围。
 */
    unsigned int number = 255U;
    printf("无符号十进制：%u\n", number);
    printf("八进制：%o\n", number);
    printf("十六进制：%x\n", number);
    printf("十六进制：%X\n", number);

    // %p 输出指针地址，参数应转换为 void *。
    printf("变量 age 的地址：%p\n", (void *)&age);

    // 输出百分号本身要写成 %%。
    printf("通过率：95%%\n");
/*
 * 格式提示：%-10s 的 10 是最小字段宽度，负号表示左对齐；%.5s 最多输出 5 个字符；宽度/精度不会改变实参。
 */
    printf("左对齐字符串：|%-10s|\n", sample);
    printf("字符串精度：%.5s\n", sample);
    printf("显式正号：%+.2f\n", pi);
    printf("sizeof(age)=%zu 字节\n", age_size);
    printf("No automatic newline:");
    printf(" the next call continues on this line.\n");

    // 一个 printf 可以按照格式顺序输出多个变量。
    printf("%s 今年 %d 岁，圆周率约 %.2f\n", name, age, pi);

    return 0;
}
