/* =========================================================
 * 文件名：03_printf_demo.c
 * 描述：printf 格式化输出常用格式符
 * 知识点：整数、字符、浮点数、字符串、进制、地址和百分号
 * ========================================================= */

#include <stdio.h>

int main(void) {
    // %d 输出十进制整数。
    int age = 18;
    printf("年龄：%d\n", age);

    // %c 输出字符；字符常量也可以按整数输出其编码值。
    char ch = 'A';
    printf("字符：%c\n", ch);
    printf("ASCII：%d\n", ch);

    // %f 输出 double；精度和字段宽度可以写在格式符中。
    double pi = 3.1415926;
    printf("圆周率：%f\n", pi);
    printf("圆周率：%.2f\n", pi);
    printf("圆周率：%8.3f\n", pi);

    // %s 输出以 \0 结尾的字符串。
    const char name[] = "张三";
    printf("姓名：%s\n", name);

    // %o、%x 和 %X 分别输出八进制、小写十六进制和大写十六进制。
    int number = 255;
    printf("八进制：%o\n", number);
    printf("十六进制：%x\n", number);
    printf("十六进制：%X\n", number);

    // %p 输出指针地址，参数应转换为 void *。
    printf("变量 age 的地址：%p\n", (void *)&age);

    // 输出百分号本身要写成 %%。
    printf("通过率：95%%\n");

    // 一个 printf 可以按照格式顺序输出多个变量。
    printf("%s 今年 %d 岁，圆周率约 %.2f\n", name, age, pi);

    return 0;
}
