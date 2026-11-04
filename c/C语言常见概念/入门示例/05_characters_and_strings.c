/* 固定数据比较字符值、零结尾字符串、NULL、strlen 和 sizeof。 */
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    char letter = 'A';
    int execution_code = 'A'; // 字符常量类型为 int；数值依执行字符集，ASCII 兼容时是 65。
    char digit = '5';

    /* C 保证数字字符连续，所以 digit - '0' 得到整数 5。 */
    int digit_value = digit - '0';

    /* word 由字面量初始化并自动补 '\0'；embedded 的 B 在首个终止符之后，空字符串也含一个终止符。 */
    char word[] = "hello";
    char embedded[] = {'A', '\0', 'B', '\0'};
    char empty_text[] = "";

    /* NULL 是空指针，不是字符串终止用的零字符；这里只比较，不解引用。 */
    const char *missing_text = NULL;

    /* strlen 数终止符前的字节；sizeof word 仍计算整个数组，包括结尾零字符。 */
    size_t word_length = strlen(word);
    size_t word_bytes = sizeof word;

    /* %c、%d 都接收 int；char 会提升为 int。size_t 长度很小，故可安全转为 int；通用输出用 %zu。 */
    printf("character = %c\n", (int)letter);
    printf("execution value of A = %d\n", execution_code);
    printf("digit = %c, value = %d\n", (int)digit, digit_value);
    printf("word = %s\n", word);
    printf("strlen(word) = %d\n", (int)word_length);
    printf("sizeof(word) = %d\n", (int)word_bytes);
    printf("embedded as string = %s\n", embedded);
    printf("strlen(embedded) = %d\n", (int)strlen(embedded));
    printf("sizeof(embedded) = %d\n", (int)sizeof embedded);
    printf("empty string length = %d\n", (int)strlen(empty_text));

    if (missing_text == NULL) {
        puts("string pointer is null");
    }

    return 0;
}

/* 示例输入：无；字符和数组内容固定。
 * 预期输出（ASCII 兼容执行字符集）：
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
 * 关键结果：'\0' 终止字符串；NULL 是指针值；strlen 与 sizeof 的计算对象不同。
 */
