/*
 * 文件用途：递归地输出汉诺塔移动步骤，并统计移动次数。
 * 输入：无；程序先演示 0 个盘，再用 3 个盘从 A 移到 C。
 * 输出：每次合法移动的盘号和柱子，以及总步数。
 * 独立编译运行：gcc -std=c11 -Wall -Wextra -Wpedantic 汉诺塔移动.c -o hanoi_recursion.exe
 */

#include <stdio.h>

/*
 * 用途：按汉诺塔规则把一叠盘从 source 移到 target。
 * 参数：disk_count 为 0 到 10；source、auxiliary、target 是互不相同的柱子标签；
 *       move_count 指向可写的移动计数器。
 * 返回值：无。
 * 异常/错误：C 不使用异常；函数不检查参数，调用者须满足盘数、柱子和指针前提。
 * 副作用：向标准输出打印每次移动，并递增 *move_count。
 * 复杂度：n 为 disk_count，递归调用和移动次数 O(2^n)，栈空间 O(n)；计入盘号文本时输出量 O(2^n log n)。
 * 用法示例：main 调用 move_hanoi(3U, 'A', 'B', 'C', &moves)，打印 7 步。
 */
static void move_hanoi(
    unsigned int disk_count,
    char source,
    char auxiliary,
    char target,
    unsigned int *move_count)
{
    if (disk_count == 0U) {
        return;
    }

    move_hanoi(disk_count - 1U, source, target, auxiliary, move_count);

    ++(*move_count);
    printf("移动第 %u 个盘：%c -> %c\n", disk_count, source, target);

    move_hanoi(disk_count - 1U, auxiliary, source, target, move_count);
}

/*
 * 用途：展示汉诺塔的空问题基准条件和一个小规模完整解。
 * 参数：无；void 表示不接收命令行参数，盘数和柱子标签固定。
 * 返回值：示例运行结束时返回 0。
 * 异常/错误：C 不使用异常；固定参数符合 move_hanoi 的前提，输出错误未检查。
 * 副作用：向标准输出打印 7 次移动和总步数。
 * 复杂度：n 为固定盘数 3，时间 O(2^n)，递归辅助空间 O(n)。
 * 用法示例：直接启动程序；0 个盘不产生移动，3 个盘产生 7 次移动，满足 2^3-1。
 */
int main(void)
{
    /* 计数器的单位是已打印的移动步骤。 */
    unsigned int zero_moves = 0U;
    puts("0 个盘：");
    move_hanoi(0U, 'A', 'B', 'C', &zero_moves);
    printf("移动次数：%u\n", zero_moves);

    unsigned int moves = 0U;
    puts("3 个盘从 A 移到 C：");
    move_hanoi(3U, 'A', 'B', 'C', &moves);
    printf("移动次数：%u，公式 2^3-1 = 7\n", moves);

    return 0;
}
