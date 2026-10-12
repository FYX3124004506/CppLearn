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
前提：values[0..count) 已按升序排列。
返回找到的下标；未找到时返回 -1。
left 和 right 表示左闭右开的搜索区间 [left, right)，
因此每次读取 values[middle] 前都有 middle < count。
*/
static ptrdiff_t binary_search(const int values[], size_t count, int target)
{
    size_t left = 0;
    size_t right = count;

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

int main(void)
{
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
