/* 固定字符串展示换行、制表、引号和反斜杠转义。 */
#include <stdio.h>

int main(void) {
    /* \n 在输出中换行；\t 请求水平制表，实际展开宽度依显示环境而异。 */
    printf("line one\nline two\n");
    printf("item\tcount\n");

    /* \" 在字符串中表示双引号；每个 \\ 在输出中产生一个反斜杠。 */
    printf("quote: \"C\"\n");
    printf("path: C:\\study\\demo.c\n");
    return 0;
}

/* 示例输入：无。
 * 预期输出：
 * line one
 * line two
 * item<TAB>count（<TAB> 表示水平制表符，展开宽度依环境而异）
 * quote: "C"
 * path: C:\study\demo.c
 * 关键结果：源码转义序列分别产生换行、制表、引号和反斜杠。
 */