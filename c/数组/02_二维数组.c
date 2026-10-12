/*
用途：演示二维数组的行列、初始化、下标、嵌套遍历和连续存储。
输入：固定示例数据；程序末尾读取 2 行、每行 3 个整数。
输出：部分初始化后的零值、平铺初始化、推导行数、各行地址和读入矩阵。
编译：gcc -std=c11 -Wall -Wextra -Wpedantic 02_二维数组.c -o array_2d
运行：./array_2d
*/
#include <stddef.h>
#include <stdio.h>

int main(void)
{
    enum { ROWS = 3, COLUMNS = 4 };

    /*
    二维数组是“数组的数组”。第一行只给出两个值，其余元素补为 0。
    通过 table[row][column] 先选行、再选列。
    */
    int table[ROWS][COLUMNS] = {
        {1, 2},
        {3, 4, 5},
        {6}
    };

    /*
    C 允许省略行花括号并按行优先填入；GCC 的 -Wmissing-braces
    提示可显式分组以提高可读性。本例保留该写法来演示平铺初始化。
    */
    int flat[2][3] = {10, 20, 30, 40, 50, 60};

    /* 可省略行数，但每行的列数必须明确。 */
    int inferred_rows[][3] = {
        {7, 8, 9},
        {10, 11, 12}
    };
    size_t inferred_row_count =
        sizeof inferred_rows / sizeof inferred_rows[0];

    printf("部分初始化的 %d 行 %d 列数组：\n", ROWS, COLUMNS);
    for (size_t row = 0; row < ROWS; ++row) {
        for (size_t column = 0; column < COLUMNS; ++column) {
            printf("%d%c", table[row][column],
                   column == COLUMNS - 1 ? '\n' : ' ');
        }
    }

    printf("平铺初始化的两行数据：\n");
    for (size_t row = 0; row < 2; ++row) {
        for (size_t column = 0; column < 3; ++column) {
            printf("%d%c", flat[row][column],
                   column == 2 ? '\n' : ' ');
        }
    }

    printf("推导出的行数：%lu\n", (unsigned long)inferred_row_count);
    printf("一行占用：%lu 字节；整个 table 占用：%lu 字节\n",
           (unsigned long)sizeof table[0],
           (unsigned long)sizeof table);

    /*
    table[0] 和 table[1] 分别指向相邻两行的首元素。
    打印地址仅供观察；具体地址和显示形式会随运行环境变化。
    */
    printf("第 0 行地址：%p\n", (void *)table[0]);
    printf("第 1 行地址：%p\n", (void *)table[1]);
    printf("第一行首元素：%d；第二行首元素：%d\n",
           table[0][0], table[1][0]);

    /* 二维输入仍逐元素读取，& 取得当前行列位置的地址。 */
    int entered[2][3];
    puts("请输入 2 行、每行 3 个整数（空格或回车分隔）：");
    for (size_t row = 0; row < 2; ++row) {
        for (size_t column = 0; column < 3; ++column) {
            if (scanf("%d", &entered[row][column]) != 1) {
                fputs("输入无效：请提供 6 个整数。\n", stderr);
                return 1;
            }
        }
    }

    puts("读入的二维数组：");
    for (size_t row = 0; row < 2; ++row) {
        for (size_t column = 0; column < 3; ++column) {
            printf("%d%c", entered[row][column],
                   column == 2 ? '\n' : ' ');
        }
    }

    return 0;
}
