/* =========================================================
 * 文件名：04_ascii_table.c
 * 描述：ASCII 编码和字符与数值之间的转换
 * 知识点：可打印字符、大小写转换、数字字符转数值
 * ========================================================= */

#include <stdio.h>

int main(void) {
    // ASCII 32 到 126 是常见 ASCII 表中的可打印字符范围。
    const int firstPrintable = 32;
    const int lastPrintable = 126;
    int column = 0;

    printf("===== ASCII 可打印字符（32~126） =====\n");
    for (int code = firstPrintable; code <= lastPrintable; ++code) {
        printf("%3d: %c   ", code, code);
        ++column;

        // 每输出 8 个字符换一行，避免表格过宽。
        if (column == 8) {
            putchar('\n');
            column = 0;
        }
    }
    if (column != 0) {
        putchar('\n');
    }
    putchar('\n');

    // 常见字符的 ASCII 值。
    printf("===== 关键 ASCII 值 =====\n");
    printf("'0'~'9' = %d ~ %d\n", '0', '9');
    printf("'A'~'Z' = %d ~ %d\n", 'A', 'Z');
    printf("'a'~'z' = %d ~ %d\n", 'a', 'z');
    printf("'\\0' = %d\n", '\0');
    printf("'\\n' = %d\n", '\n');
    printf("空格   = %d\n", ' ');
    putchar('\n');

    /*
     * 下面的加减法依赖 ASCII 中大小写字母的连续编码关系。
     * 在实际工程中也可以使用 <ctype.h> 中的转换函数。
     */
    printf("===== 大小写转换 =====\n");
    const char upper = 'B';
    const char lower = (char)(upper + ('a' - 'A'));
    printf("%c 转小写：%c\n", upper, lower);

    const char lowerInput = 'd';
    const char upperOutput = (char)(lowerInput - ('a' - 'A'));
    printf("%c 转大写：%c\n", lowerInput, upperOutput);
    putchar('\n');

    // 数字字符减去字符 '0'，可以得到对应的整数值。
    printf("===== 数字字符转数值 =====\n");
    const char digitChar = '7';
    const int digit = digitChar - '0';
    printf("字符 '%c' 转数值：%d\n", digitChar, digit);
    printf("数字字符 '5' + '3'：%d\n", '5' - '0' + '3' - '0');

    return 0;
}
