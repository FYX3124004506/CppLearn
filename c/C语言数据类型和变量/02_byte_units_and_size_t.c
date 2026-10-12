/*
 * 演示 C 字节、size_t，以及数组与指针的区别。
 * 输入：无。使用 C11 编译后运行。
 */
#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <limits.h>
#include <stddef.h>
#include <stdio.h>

int main(void)
{
    int values[] = {10, 20, 30, 40};
    int *pointer = values;
    size_t array_bytes = sizeof values;
    size_t element_bytes = sizeof values[0];
    size_t element_count = array_bytes / element_bytes;
    int probe = 4;
    size_t unevaluated_size = sizeof(probe++);

    printf("sizeof(char)=%zu; CHAR_BIT=%d bits per C byte\n", sizeof(char), CHAR_BIT);
    printf("sizeof(int)=%zu; sizeof probe=%zu\n", sizeof(int), sizeof probe);
    int rank_sizes_nondecreasing =
        sizeof(short) <= sizeof(int) && sizeof(int) <= sizeof(long) &&
        sizeof(long) <= sizeof(long long);
    printf("integer type sizes nondecreasing=%s\n",
           rank_sizes_nondecreasing ? "yes" : "no");
    printf("array bytes=%zu; element bytes=%zu; elements=%zu\n",
           array_bytes, element_bytes, element_count);
    printf("pointer bytes=%zu; sizeof *pointer=%zu; first value=%d\n",
           sizeof pointer, sizeof *pointer, *pointer);
    printf("sizeof(probe++)=%zu; probe remains %d\n", unevaluated_size, probe);
    /* sizeof(char) 等于一个 C 字节；CHAR_BIT 可查询一个字节包含的比特数。 */
    /* 类型的具体大小随实现而异；sizeof 后接类型名时必须加括号。 */
    return 0;
}