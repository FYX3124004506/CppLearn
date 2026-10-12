/* =========================================================
 * 文件名：04_ascii_table.c
 * 描述：ASCII 编码和字符与数值之间的转换
 * 知识点：可打印字符、大小写转换、数字字符转数值
 * ========================================================= */

/*
 * 文件用途：打印 ASCII 可打印码表，并示范字符数值、大小写和数字字符转换。
 * 输入：无；字符和码值都由程序固定给出。
 * 输出：标准输出显示 32 到 126 的表格及若干转换示例。
 */
#include <stdio.h>

/*
 * 用途：遍历可打印 ASCII 码并输出字符转换示例。
 * 参数：无；码值区间由常量设定。
 * 返回值：int；返回 0 表示正常结束。
 * 异常/错误：C 不使用异常机制；没有外部输入；输出函数失败未检查，程序固定返回 0。
 * 副作用：向标准输出写入码表和转换文本。
 * 复杂度：时间 O(N)，N 为码值个数，本例 N=126-32+1=95；辅助空间 O(1)，只使用循环计数和列计数。
 * 示例：运行本程序会输出 32 到 126 的码值，并实际计算大写转小写及数字字符转数值。
 */
int main(void) {
    // ASCII 32 到 126 是常见 ASCII 表中的可打印字符范围。
/*
 * 关键变量：firstPrintable 和 lastPrintable 是码值边界（无物理单位），分别为 32、126；column 是当前行已输出字符数，范围为 0 到 8；达到 8 后换行并重置为 0。
 */
    const int firstPrintable = 32;
    const int lastPrintable = 126;
    int column = 0;

    printf("===== ASCII 可打印字符（32~126） =====\n");
/*
 * 编码说明：此处把 32 到 126 当作 ASCII 可打印码值；把 code 交给 %c 显示字符依赖目标执行字符集与 ASCII 相容。
 */
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
/*
 * 边界说明：大小写加减写法依赖 ASCII 字母区间的连续关系，不能无条件推广到所有执行字符集；本例固定用 ASCII 示例字符。
 */
    const char upper = 'B';
    const char lower = (char)(upper + ('a' - 'A'));
    printf("%c 转小写：%c\n", upper, lower);

    const char lowerInput = 'd';
    const char upperOutput = (char)(lowerInput - ('a' - 'A'));
    printf("%c 转大写：%c\n", lowerInput, upperOutput);
    putchar('\n');

    // 数字字符减去字符 '0'，可以得到对应的整数值。
    printf("===== 数字字符转数值 =====\n");
/*
 * 范围说明：digitChar 必须是 '0' 到 '9' 的十进制数字字符；字符集保证这些数字字符连续，因此相减可得 0 到 9 的数值。
 */
    const char digitChar = '7';
    const int digit = digitChar - '0';
    printf("字符 '%c' 转数值：%d\n", digitChar, digit);
    printf("数字字符 '5' + '3'：%d\n", '5' - '0' + '3' - '0');

    return 0;
}
