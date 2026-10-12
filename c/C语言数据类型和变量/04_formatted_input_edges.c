/*
 * 使用固定输入演示 scanf 扫描集和 %n。
 * 输入：无。使用 C11 编译后运行。
 */
#define __STDC_WANT_LIB_EXT1__ 1
#include <stdio.h>

int main(void)
{
    const char text[] = " ABC";
    char without_space[8] = "";
    char after_space[8] = "";

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