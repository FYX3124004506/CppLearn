/*
 * 使用固定输入演示 scanf 扫描集和 %n。
 * 输入：无。使用 C11 编译后运行。
 */
/* 输出：固定字符串扫描的赋值数、扫描结果、%n 字符计数和 Annex K 可用性。 */
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

/*
 * 用途：用 sscanf 演示扫描集的空白处理、部分匹配和 %n 的计数规则。
 * 参数：无。
 * 返回值：0 表示示例正常结束。
 * 异常/错误：C 无异常机制；扫描结果通过 sscanf 的赋值计数观察，固定输入保证缓冲区有界。
 * 副作用：向 stdout 输出结果；只读取程序内固定字符串，不读取 stdin。
 * 复杂度：时间 O(1)，辅助空间 O(1)；输入字符串和缓冲区长度均固定。
 * 示例：运行本文件进入 main(void)，比较 raw_result 与 spaced_result，再观察 consumed。
 */
int main(void)
{
    /* 两个目标缓冲区各 8 字节，含结尾 NUL；%7 扫描至多写入 7 个字符。 */
    const char text[] = " ABC";
    char without_space[8] = "";
    char after_space[8] = "";

    /* %[ 不像 %d 那样自动跳过前导空白；格式串显式空格才会先消费空白。 */
    int raw_result = sscanf(text, "%7[ABCDEFGHIJKLMNOPQRSTUVWXYZ]", without_space);
    int spaced_result = sscanf(text, " %7[ABCDEFGHIJKLMNOPQRSTUVWXYZ]", after_space);
    printf("%%[ does not skip whitespace: result=%d; leading format space: result=%d, text=%s\n",
           raw_result, spaced_result, after_space);

    int year = 0;
    int month = 0;
    int date_assignments = sscanf("2026-10", "%d-%d", &year, &month);
    printf("literal date assignments=%d; year=%d; month=%d\n",
           date_assignments, year, month);

    int value = 0;
    int consumed = -1;
    int assignments = sscanf("42 rest", "%d%n", &value, &consumed);
    printf("%%n consumed=%d; counted assignments=%d; value=%d\n",
           consumed, assignments, value);
    /* %n 保存已经读取的字符数，但不增加 scanf 的成功赋值计数。 */
    /* 先请求可选的 Annex K 接口，再查询实现是否公开支持；不支持时仍使用标准 scanf 接口。 */
#if defined(__STDC_LIB_EXT1__)
    puts("This implementation advertises the optional Annex K interfaces.");
#else
    puts("This implementation does not advertise Annex K; standard scanf remains available.");
#endif
    return 0;
}