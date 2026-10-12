/*
 * 演示 C 字节、size_t，以及数组与指针的区别。
 * 输入：无。使用 C11 编译后运行。
 */
/* 输出：当前实现的类型大小、数组字节数/元素数、指针大小及 sizeof 求值示例。 */
#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <limits.h>
#include <stddef.h>
#include <stdio.h>

/*
 * 用途：观察 sizeof、CHAR_BIT、size_t，以及数组与指针的区别。
 * 参数：无。
 * 返回值：0 表示示例正常结束。
 * 异常/错误：C 无异常机制；本例无输入和显式错误路径，标准输出错误未检查。
 * 副作用：向 stdout 输出实现相关的类型与对象大小。
 * 复杂度：时间 O(1)，辅助空间 O(1)；数组元素数固定为 4，未按输入规模遍历。
 * 示例：运行本文件进入 main(void)，比较 array_bytes、element_bytes 和 element_count。
 */
int main(void)
{
    /* element_count 的单位是 int 元素个数；array_bytes 与 element_bytes 的单位是 C 字节。 */
    int values[] = {10, 20, 30, 40};
    int *pointer = values;
    size_t array_bytes = sizeof values;
    size_t element_bytes = sizeof values[0];
    size_t element_count = array_bytes / element_bytes;
    /* probe 是 sizeof(probe++) 的演示对象；该非 VLA sizeof 操作数不求值，值仍为 4。 */
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