/* =========================================================
 * 文件名：05_string_and_null.c
 * 描述：字符串与 \0
 * 知识点：字符数组、字符串结束标志、strlen 和 sizeof
 * ========================================================= */

/*
 * 文件用途：比较 C 字符串的终止空字符、strlen 长度和 sizeof 数组大小。
 * 输入：无；所有字符数组都在程序中初始化。
 * 输出：标准输出显示字符串及其长度/大小关系。
 */
#include <stdio.h>
#include <string.h>

/*
 * 用途：通过固定字符数组展示字符串必须以空字符结尾，并对照 strlen 与 sizeof。
 * 参数：无；示例数据在函数内定义。
 * 返回值：int；返回 0 表示正常结束。
 * 异常/错误：C 不使用异常机制；本例不读外部输入，并避免把未终止数组交给 %s；输出错误未检查。
 * 副作用：向标准输出写入字符串和长度；只修改 hello 的局部元素 hello[5]。
 * 复杂度：时间 O(N)，N 为扫描/输出的字符数，样例最长 11 个有效字符；辅助空间 O(1)，另有 O(N) 的固定局部数组存储。
 * 示例：main 实际调用 strlen(str1)、strlen(text)，并在 hello[5] 写入空字符后调用 printf 输出截断结果。
 */
int main(void) {
    /*
     * 当前 Windows C 运行库对 %zu 的支持不一致；本示例中的长度很小，
     * 转换为 unsigned long 后使用 %lu 可以兼容该环境并保持输出正确。
     */
    // 双引号字符串字面量会自动在末尾追加 \0。
/*
 * 数组范围：str1 有 6 个 char 元素，包含 5 个可见字符和结尾空字符；在本例字符均为基本字符。
 */
    char str1[] = "hello";
    printf("str1 = \"%s\"  sizeof = %lu  strlen = %lu\n",
           str1,
           (unsigned long)sizeof str1,
           (unsigned long)strlen(str1));

    // 使用字符数组初始化时，需要手动写入 \0。
    char str2[] = {'h', 'e', 'l', 'l', 'o', '\0'};
    printf("str2 = \"%s\"  sizeof = %lu  strlen = %lu\n",
           str2,
           (unsigned long)sizeof str2,
           (unsigned long)strlen(str2));
    putchar('\n');

    /*
     * 对数组而言，strlen 统计 \0 之前的字符数；
     * sizeof 统计整个数组的字节数，因此包含末尾的 \0。
     */
/*
 * 长度说明：text 的 strlen 结果是 6 个字符，sizeof text 是 7 字节；sizeof 的单位始终是字节。
 */
    char text[] = "ABCDEF";
    printf("字符串：%s\n", text);
    printf("strlen(text) = %lu（不包含 \\0）\n",
           (unsigned long)strlen(text));
    printf("sizeof text = %lu（包含 \\0）\n",
           (unsigned long)sizeof text);
    putchar('\n');

    // 提前写入 \0 会让字符串输出在该位置结束。
/*
 * 边界说明：hello 的索引从 0 开始，hello[5] 原本是逗号；改为空字符后，%s 在第 5 个索引处停止。
 */
    char hello[] = "hello,world";
    hello[5] = '\0';
    printf("放入 \\0 后：%s\n", hello);
    putchar('\n');

    /*
     * 没有 \0 的字符数组不是 C 字符串，使用 %s 读取它会越界，
     * 结果属于未定义行为，因此这里只声明而不直接打印。
     */
/*
 * 范围说明：badString 只有 4 个元素且没有结尾空字符；不能把它当作 %s 所需的 C 字符串。
 */
    char badString[] = {'h', 'i', '!', '!'};
    (void)badString;

    // 补上 \0 后，字符数组才可以安全地作为字符串使用。
    char goodString[] = {'h', 'i', '!', '!', '\0'};
    printf("补上 \\0 后：%s\n", goodString);

    return 0;
}
