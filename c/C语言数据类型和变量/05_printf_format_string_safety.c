/*
 * 演示如何把包含百分号的文本作为数据安全输出。
 * 输入：无。使用 C11 编译后运行。
 */
/* 输出：安全传入的百分号文本、字符串字节数，以及条件编译的 UTF-8 精度示例。 */
#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/*
 * 用途：演示固定 printf 格式串、百分号数据和字符串精度的安全边界。
 * 参数：无。
 * 返回值：0 表示示例正常结束。
 * 异常/错误：C 无异常机制；snprintf 的返回值被记录并输出，标准输出错误未检查。
 * 副作用：向 stdout 输出固定文本；snprintf(NULL, 0, ...) 仅计算所需字节数。
 * 复杂度：时间 O(L)，L 为固定 message/UTF-8 示例文本长度；辅助空间 O(L)，保存固定字节数组和文本。
 * 示例：运行本文件进入 main(void)，先看 message 经 %s 输出，再观察 %.2s 的字节计数。
 */
int main(void)
{
    const char message[] = "score=100% ready";
    /* message_bytes 单位为字节且不含 NUL；本例值为 sizeof(message)-1。 */
    size_t message_bytes = sizeof message - 1;

    /* 固定格式字符串控制输出；message 中的百分号由 %s 当作普通文本处理。 */
    printf("文本：%s\n", message);
    printf("文本占用 %zu 个字节\n", message_bytes);
    printf("格式中的百分号要写成 %%：100%%\n");

#if CHAR_BIT == 8
    /* utf8_bytes 是 8 位字节序列；%.2s 的精度按字节计，可能截断多字节字符。 */
    const unsigned char utf8_bytes[] = {0xE4U, 0xB8U, 0xADU, 0x41U, 0U};
    char utf8_text[sizeof utf8_bytes];
    memcpy(utf8_text, utf8_bytes, sizeof utf8_bytes);
    int bytes_would_be_written = snprintf(NULL, 0, "%.2s", utf8_text);
    printf("printf string precision=2 would write %d bytes; raw prefix omitted\n",
           bytes_would_be_written);
#else
    puts("UTF-8 precision example skipped because this target does not use 8-bit bytes.");
#endif

    /* 不把外部文本直接传给 printf 当格式字符串，避免把其中的 % 当成转换说明。 */
    return 0;
}