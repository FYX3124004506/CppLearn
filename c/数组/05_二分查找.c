/*
用途：在升序数组中演示二分查找、边界处理和 sizeof 元素个数计算。
输入：无；使用固定的升序整数数组。
输出：首尾元素、存在目标、不存在目标及空逻辑区间的查找结果。
编译：gcc -std=c11 -Wall -Wextra -Wpedantic 05_二分查找.c -o array_binary_search
运行：./array_binary_search
*/
#include <stddef.h>
#include <stdio.h>

/*
用途：在升序整数数组中查找 target，并返回一个匹配元素的下标。
参数：values 指向 count 个按非降序排列的 int 元素；count 是元素个数；target 是待查整数。
返回值：找到时返回可由 ptrdiff_t 表示的下标；未找到时返回 -1。
异常/错误：C 不使用异常机制；-1 表示未找到。函数不检查排序或指针，count>0 时调用者须传入有效数组；count=0 时不会解引用 values。
副作用：无；只读取数组和参数。
复杂度：令 n=count；二分区间每轮至少缩小一半，时间 O(log n)（n=0 时常量时间），辅助空间 O(1)。
示例：show_search_result 内实际调用 binary_search(values, count, target)，并按返回值区分找到与未找到。
*/
/*
前提：values[0..count) 已按升序排列。
返回找到的下标；未找到时返回 -1。
left 和 right 表示左闭右开的搜索区间 [left, right)，
因此每次读取 values[middle] 前都有 middle < count。
*/
static ptrdiff_t binary_search(const int values[], size_t count, int target)
{
    size_t left = 0;
    size_t right = count;

    /* 搜索区间为 [left, right)；中点公式避免直接计算 left+right，分支则丢弃不可能包含目标的一半。 */
    while (left < right) {
        size_t middle = left + (right - left) / 2;

        if (values[middle] == target) {
            return (ptrdiff_t)middle;
        }
        if (values[middle] < target) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }

    return -1;
}

/*
用途：调用 binary_search 并把命中下标或未命中消息打印出来。
参数：values 是待搜索数组；count 是元素个数；target 是目标整数；count>0 时 values 必须有效且升序。
返回值：无（void）。
异常/错误：C 不使用异常机制；将 binary_search 返回的 -1 作为未找到信号显示，不另行验证数组前提。
副作用：向 stdout 输出一条查找结果。
复杂度：令 n=count；时间 O(log n)（空数组为 O(1)），辅助空间 O(1)。
示例：main 中 show_search_result(sorted, count, 2) 展示首元素命中的情况。
*/
static void show_search_result(const int values[],
                               size_t count,
                               int target)
{
    ptrdiff_t position = binary_search(values, count, target);

    if (position >= 0) {
        printf("目标 %d 位于下标 %ld\n", target, (long)position);
    } else {
        printf("目标 %d 不在数组中\n", target);
    }
}

/*
用途：用固定升序数组展示查找首个、中间、末尾、不存在目标和空区间。
参数：无（main 的参数列表为 void）。
返回值：演示完成后返回 0。
异常/错误：C 不使用异常机制；示例数据固定且无需外部输入，空区间示例由 binary_search 返回 -1 处理。
副作用：向 stdout 输出数组长度及 5 条查找结果。
复杂度：令 n 为数组长度、k 为演示次数；共 k=5 次查找，时间 O(k log n)，本例规模固定所以实际为 O(1)；辅助空间 O(1)，另有 n 个元素的固定示例数组。
示例：入口依次调用 show_search_result(sorted, count, 2)、show_search_result(sorted, count, 12) 等展示不同边界。
*/
int main(void)
{
    /* sorted 的元素数由初始化项决定；count 的单位是元素个数，本例为 6。 */
    int sorted[] = {2, 5, 8, 12, 16, 23};
    size_t count = sizeof sorted / sizeof sorted[0];

    printf("数组长度：%lu\n", (unsigned long)count);
    show_search_result(sorted, count, 2);   /* 第一个元素 */
    show_search_result(sorted, count, 12);  /* 中间元素 */
    show_search_result(sorted, count, 23);  /* 最后一个元素 */
    show_search_result(sorted, count, 7);   /* 不存在的目标 */

    /*
    count 为 0 表示空的逻辑区间，不是声明零长度数组。
    循环不进入，不会解引用 NULL 指针。
    */
    show_search_result(NULL, 0, 9);

    return 0;
}
