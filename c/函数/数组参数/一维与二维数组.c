/*
 * 文件用途：展示一维数组形参、显式长度、二维数组列宽和 const 只读访问。
 * 输入：固定的一维三元素数组、固定的二行三列矩阵，并检查空指针边界。
 * 输出：修改后的一维数组、逐行矩阵及边界检查结果。
 * 独立运行：gcc -std=c11 -Wall -Wextra -Wpedantic 一维与二维数组.c -o 一维与二维数组.exe
 */
#include <stddef.h>
#include <stdio.h>

/*
 * 用途：给一维数组的每个元素增加固定增量。
 * 参数：values 指向首元素；count 是要处理的元素数，必须不大于实际数组长度；
 *       increment 是增量，要求每次加法结果都可由 int 表示。
 * 返回值：处理成功返回 1；count 大于 0 且 values 为空时返回 0。
 * 异常/错误：C 不使用异常；检查空指针，但无法从指针得知数组真实长度，长度与算术范围由调用者保证。
 * 副作用：修改 values 指向的 count 个数组元素。
 * 复杂度：时间 O(n)，n=count；辅助空间 O(1)。
 * 示例：main 调用 add_to_each(values, 3, 10)，把 {1,2,3} 改成 {11,12,13}。
 */
int add_to_each(int values[], size_t count, int increment)
{
    size_t index;

    if (count > 0U && values == NULL) {
        return 0;
    }

    for (index = 0U; index < count; ++index) {
        values[index] += increment;
    }

    return 1;
}

/*
 * 用途：只读输出具有固定三列的二维整数矩阵。
 * 参数：row_count 是行数，必须不大于实参数组的实际行数；
 *       matrix 指向每行有 3 个 int 元素的矩阵，可在 row_count 为 0 时为空。
 * 返回值：全部输出成功返回 1；参数不满足要求或标准输出失败时返回 0。
 * 异常/错误：C 不使用异常；检查空指针并检查 printf 的负返回值；不能自动验证真实行数。
 * 副作用：向标准输出写入矩阵元素，不修改矩阵。
 * 复杂度：时间 O(r*c)，r=row_count、c=3；辅助空间 O(1)。
 * 示例：main 调用 print_matrix(2, matrix)，输出两行三列的固定矩阵。
 */
int print_matrix(size_t row_count, const int matrix[][3])
{
    size_t row;
    size_t column;

    if (row_count > 0U && matrix == NULL) {
        return 0;
    }

    for (row = 0U; row < row_count; ++row) {
        for (column = 0U; column < 3U; ++column) {
            int result = printf("%d%c",
                                 matrix[row][column],
                                 column == 2U ? '\n' : ' ');
            if (result < 0) {
                return 0;
            }
        }
    }

    return 1;
}

/*
 * 用途：运行受控的一维与二维数组参数示例及空指针边界检查。
 * 参数：无；数组长度和矩阵尺寸都在本函数中固定。
 * 返回值：int；所有检查与输出成功返回 0，否则返回 1。
 * 异常/错误：C 不使用异常；数组修改函数和矩阵输出函数以状态值报告受控错误。
 * 副作用：修改本地 values 数组并向标准输出写入结果。
 * 复杂度：时间 O(n+r*c)，n=3、r=2、c=3；辅助空间 O(n+r*c)，仅存放固定数组。
 * 示例：启动程序即可观察数组修改、二维矩阵布局以及空指针被拒绝。
 */
int main(void)
{
    int values[] = {1, 2, 3};
    const int matrix[2][3] = {
        {1, 2, 3},
        {4, 5, 6}
    };
    size_t value_count = sizeof values / sizeof values[0];
    int rejected_null_values;
    int rejected_null_matrix;

    if (!add_to_each(values, value_count, 10)) {
        return 1;
    }

    if (printf("one-dimensional result: %d %d %d\n",
               values[0], values[1], values[2]) < 0) {
        return 1;
    }

    if (printf("two-dimensional result:\n") < 0 || !print_matrix(2U, matrix)) {
        return 1;
    }

    rejected_null_values = !add_to_each(NULL, 1U, 10);
    rejected_null_matrix = !print_matrix(1U, NULL);

    if (printf("null array rejected: %s\n",
               rejected_null_values ? "yes" : "no") < 0) {
        return 1;
    }
    if (printf("null matrix rejected: %s\n",
               rejected_null_matrix ? "yes" : "no") < 0) {
        return 1;
    }

    return 0;
}
