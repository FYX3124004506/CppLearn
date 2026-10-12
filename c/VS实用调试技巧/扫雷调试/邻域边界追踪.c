/*
 * 文件用途：在固定 5×5 棋盘上输出邻雷数，并追踪角落、边缘、中心的邻格检查。
 * 输入：无外部输入；棋盘布局固定，便于重复运行和对照预期结果。
 * 输出：完整棋盘结果，以及选定格子的候选邻居和相邻雷数。
 * 独立编译：gcc -std=c11 -Wall -Wextra -Wpedantic 邻域边界追踪.c -o minesweeper_trace.exe
 * 运行：启动程序；在 count_adjacent_mines 的 ++count 行设置断点观察状态。
 * 调试：监视 row、column、row_offset、column_offset、neighbor_row、neighbor_column 和 count。
 * 安全：越界候选只做边界判断；is_mine_at 使用 && 短路保护二维数组访问。
 */
#include <stdio.h>

enum
{
    ROWS = 5,
    COLUMNS = 5
};

static const char board[ROWS][COLUMNS] = {
    {'.', '.', '.', '.', '.'},
    {'.', '*', '.', '.', '.'},
    {'.', '.', '.', '*', '.'},
    {'.', '.', '.', '.', '.'},
    {'*', '.', '.', '.', '.'}
};

/*
 * 用途：判断行列坐标是否位于固定棋盘范围。
 * 参数：row、column 是从 0 开始的有符号候选坐标，可包含边界外值。
 * 返回值：坐标都有效返回 1，否则返回 0。
 * 异常/错误：C 不使用异常；任意 int 坐标都可安全处理。
 * 副作用：无。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 用法：count_adjacent_mines 和 print_board 在读取棋盘前校验坐标或使用固定有效坐标。
 */
static int is_inside_board(int row, int column)
{
    return row >= 0 && row < ROWS && column >= 0 && column < COLUMNS;
}

/*
 * 用途：判断候选坐标是否为雷，并用短路规则保护二维数组访问。
 * 参数：row、column 是从 0 开始的任意 int 坐标，越界坐标允许传入。
 * 返回值：坐标在棋盘内且对应字符为 '*' 时返回 1，否则返回 0。
 * 异常/错误：C 不使用异常；边界外坐标被安全判为非雷。
 * 副作用：无。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 用法：count_adjacent_mines 对候选邻格调用；边界条件不成立时 && 不会求值数组访问项。
 */
static int is_mine_at(int row, int column)
{
    return row >= 0 && row < ROWS &&
           column >= 0 && column < COLUMNS &&
           board[row][column] == '*';
}

/*
 * 用途：统计安全格周围最多八个方向的雷数，并可选择打印候选邻格轨迹。
 * 参数：row、column 是棋盘坐标；show_trace 为 0 时静默计数，非 0 时打印候选状态。
 * 前提：目标坐标必须有效且不是雷；越界或雷格作为起点会返回错误值。
 * 返回值：有效安全格返回 0..8 的相邻雷数；无效起点返回 -1。
 * 异常/错误：C 不使用异常；无效起点用 -1 报告；候选越界坐标会安全跳过。
 * 副作用：show_trace 非 0 时向标准输出打印候选坐标状态。
 * 复杂度：最多检查 8 个候选，时间 O(1)，辅助空间 O(1)。
 * 用法：print_board 传 0；main 为角落、边缘和中心传 1 观察逐步检查过程。
 */
static int count_adjacent_mines(int row, int column, int show_trace)
{
    if (!is_inside_board(row, column) || board[row][column] == '*')
    {
        return -1;
    }

    int count = 0;

    for (int row_offset = -1; row_offset <= 1; ++row_offset)
    {
        for (int column_offset = -1; column_offset <= 1; ++column_offset)
        {
            if (row_offset == 0 && column_offset == 0)
            {
                continue;
            }

            const int neighbor_row = row + row_offset;
            const int neighbor_column = column + column_offset;
            const int inside = is_inside_board(neighbor_row, neighbor_column);
            const int has_mine = is_mine_at(neighbor_row, neighbor_column);

            if (!inside)
            {
                if (show_trace)
                {
                    printf("候选 (%d,%d)：越界，安全跳过\n",
                           neighbor_row, neighbor_column);
                }
                continue;
            }

            if (show_trace)
            {
                printf("候选 (%d,%d)：%s\n",
                       neighbor_row, neighbor_column,
                       has_mine ? "雷" : "安全格");
            }

            if (has_mine)
            {
                ++count;
            }
        }
    }

    return count;
}

/*
 * 用途：按固定棋盘逐格输出地雷和每个安全格周围的雷数。
 * 参数：无；棋盘行列数由 ROWS 和 COLUMNS 固定。
 * 返回值：无。
 * 异常/错误：C 不使用异常；仅以有效棋盘坐标调用计数函数。
 * 副作用：向标准输出写入 5 行棋盘结果。
 * 复杂度：R、C 分别为行数和列数；时间 O(R×C)，辅助空间 O(1)。
 * 用法：main 在逐个追踪邻格前调用，输出应与固定棋盘的预期表格一致。
 */
static void print_board(void)
{
    puts("固定棋盘计算结果：");

    for (int row = 0; row < ROWS; ++row)
    {
        for (int column = 0; column < COLUMNS; ++column)
        {
            if (board[row][column] == '*')
            {
                putchar('*');
            }
            else
            {
                const int adjacent_count = count_adjacent_mines(row, column, 0);
                putchar('0' + adjacent_count);
            }

            putchar(column == COLUMNS - 1 ? '\n' : ' ');
        }
    }
}

/*
 * 用途：核对固定棋盘的已知结果，并逐步追踪角落、边缘、中心和无效起点。
 * 参数：无。
 * 返回值：所有预期值匹配返回 0；否则返回 1。
 * 异常/错误：C 不使用异常；计数与预期不符时用状态码 1 报告。
 * 副作用：调用计数函数并向标准输出写调试轨迹。
 * 复杂度：棋盘输出 O(R×C)，额外三次邻域检查为 O(1)；辅助空间 O(1)。
 * 用法：直接运行；输出棋盘后应观察角落=1、边缘=1、中心=2、越界起点=-1。
 */
int main(void)
{
    print_board();

    puts("追踪左上角 (0,0)：");
    const int corner_count = count_adjacent_mines(0, 0, 1);

    puts("追踪上边缘 (0,2)：");
    const int edge_count = count_adjacent_mines(0, 2, 1);

    puts("追踪中心 (2,2)：");
    const int center_count = count_adjacent_mines(2, 2, 1);

    const int invalid_start = count_adjacent_mines(-1, 0, 1);

    printf("结果：角落=%d（预期 1），边缘=%d（预期 1），中心=%d（预期 2），越界起点=%d（预期 -1）\n",
           corner_count, edge_count, center_count, invalid_start);

    if (corner_count != 1 || edge_count != 1 ||
        center_count != 2 || invalid_start != -1)
    {
        return 1;
    }

    return 0;
}
