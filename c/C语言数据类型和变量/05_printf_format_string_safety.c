/*
 * 演示如何把包含百分号的文本作为数据安全输出。
 * 输入：无。使用 C11 编译后运行。
 */
#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    const char message[] = "score=100% ready";
    size_t message_bytes = sizeof message - 1;

    /* 固定格式字符串控制输出；message 中的百分号由 %s 当作普通文本处理。 */
    printf("文本：%s\n", message);
    printf("文本占用 %zu 个字节\n", message_bytes);
    printf("格式中的百分号要写成 %%：100%%\n");

#if CHAR_BIT == 8
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