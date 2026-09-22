/* =========================================================
 * 文件名：05_string_and_null.c
 * 描述：字符串与 \0
 * 知识点：字符数组、字符串结束标志、strlen 和 sizeof
 * ========================================================= */

#include <stdio.h>
#include <string.h>

int main(void) {
    // 双引号字符串字面量会自动在末尾追加 \0。
    char str1[] = "hello";
    printf("str1 = \"%s\"  sizeof = %u  strlen = %u\n",
           str1,
           (unsigned int)sizeof str1,
           (unsigned int)strlen(str1));

    // 使用字符数组初始化时，需要手动写入 \0。
    char str2[] = {'h', 'e', 'l', 'l', 'o', '\0'};
    printf("str2 = \"%s\"  sizeof = %u  strlen = %u\n",
           str2,
           (unsigned int)sizeof str2,
           (unsigned int)strlen(str2));
    putchar('\n');

    /*
     * 对数组而言，strlen 统计 \0 之前的字符数；
     * sizeof 统计整个数组的字节数，因此包含末尾的 \0。
     */
    char text[] = "ABCDEF";
    printf("字符串：%s\n", text);
    printf("strlen(text) = %u（不包含 \\0）\n",
           (unsigned int)strlen(text));
    printf("sizeof text = %u（包含 \\0）\n",
           (unsigned int)sizeof text);
    putchar('\n');

    // 提前写入 \0 会让字符串输出在该位置结束。
    char hello[] = "hello,world";
    hello[5] = '\0';
    printf("放入 \\0 后：%s\n", hello);
    putchar('\n');

    /*
     * 没有 \0 的字符数组不是 C 字符串，使用 %s 读取它会越界，
     * 结果属于未定义行为，因此这里只声明而不直接打印。
     */
    char badString[] = {'h', 'i', '!', '!'};
    (void)badString;

    // 补上 \0 后，字符数组才可以安全地作为字符串使用。
    char goodString[] = {'h', 'i', '!', '!', '\0'};
    printf("补上 \\0 后：%s\n", goodString);

    return 0;
}
