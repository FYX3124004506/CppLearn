/*
 * 文件用途：用 9×9 棋盘演示数组状态、随机布雷、邻雷统计、空白区域展开和输入验证。
 * 输入：普通模式每回合输入 1..9 范围内的“行 列”；--algorithm-demo 无需输入。
 * 输出：普通模式显示游戏棋盘；演示模式显示固定布局的边界计数和空白展开。
 * 独立编译：gcc -std=c11 -Wall -Wextra -Wpedantic 命令行扫雷.c -o minesweeper
 * 运行：无参数启动游戏；传入 --algorithm-demo 查看固定棋盘算法演示。
 */
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum {
    ROWS = 9,
    COLUMNS = 9,
    CELL_COUNT = ROWS * COLUMNS,
    MINE_COUNT = 10,
    RANDOM_SAMPLE_COUNT = 3,
    SAFE_CELL_COUNT = CELL_COUNT - MINE_COUNT,
    INPUT_ERROR = -2,
    INPUT_INVALID = -1,
    INPUT_END = 0,
    INPUT_OK = 1
};

/* ========== 随机位置选择：将 rand() 安全映射到一个有限下标区间 ========== */

/*
 * 用途：在 [0, upper_bound) 中产生一个 rand() 伪随机下标，并拒绝不完整余数区间。
 * 参数：upper_bound 是上界（不含），有效范围为 1..RAND_MAX+1；本程序传入 72..81。
 * 返回值：有效时返回 0..upper_bound-1；上界无效时返回 0。
 * 异常/错误：C 不使用异常；rand 序列质量由实现决定，不适合安全、抽奖或密码用途；无效上界返回 0，实际调用的上界始终有效。
 * 副作用：有效调用会消耗 rand() 的全局伪随机状态；无效上界不调用 rand()。
 * 复杂度：期望时间 O(1)，最坏重抽次数没有有限上界；辅助空间 O(1)。
 * 示例：place_mines 每轮用 81-index 作为上界，选择当前洗牌位置之后的一个格子。
 */
static unsigned int random_below(unsigned int upper_bound)
{
    const unsigned int random_range = (unsigned int)RAND_MAX + 1U;

    if (upper_bound == 0U || upper_bound > random_range) {
        return 0U;
    }

    /*
     * accepted_limit 是 random_range 的最大完整 upper_bound 倍数。
     * 丢弃其后的尾部值后，取模不会因尾部多出几个值而偏向某些余数。
     */
    const unsigned int accepted_limit =
        random_range - random_range % upper_bound;
    unsigned int value;

    do {
        value = (unsigned int)rand();
    } while (value >= accepted_limit);

    return value % upper_bound;
}

/* ========== 随机种子演示：同种子在同一实现中重现序列 ========== */

/*
 * 用途：比较同一 unsigned int 种子生成的两个固定长度 rand 序列前缀。
 * 参数：seed 是 srand 接受的 unsigned int 种子值。
 * 返回值：无。
 * 异常/错误：C 不使用异常；rand/srand 没有错误返回值。
 * 副作用：多次重置并消耗 rand 的全局状态；向 stdout 输出序列是否一致。
 * 复杂度：K 为样本个数，时间 O(K)，辅助空间 O(K) 保存两组样本；本例 K=3。
 * 示例：main 传入当前时间转换出的 game_seed；随后再次 srand(game_seed) 以重置游戏序列。
 */
static void show_seed_repeatability(unsigned int seed)
{
    int first[RANDOM_SAMPLE_COUNT];
    int second[RANDOM_SAMPLE_COUNT];

    srand(seed);
    for (int index = 0; index < RANDOM_SAMPLE_COUNT; ++index) {
        first[index] = rand();
    }

    srand(seed);
    for (int index = 0; index < RANDOM_SAMPLE_COUNT; ++index) {
        second[index] = rand();
    }

    int sequences_match = 1;
    for (int index = 0; index < RANDOM_SAMPLE_COUNT; ++index) {
        if (first[index] != second[index]) {
            sequences_match = 0;
        }
    }

    printf("相同种子重复序列一致：%s\n",
           sequences_match ? "是" : "否");
}

/* ========== 棋盘数据与布雷：把一维位置编号转换为二维数组坐标 ========== */

/*
 * 用途：清空棋盘并将恰好 MINE_COUNT 个雷放到不同格子。
 * 参数：mines 指向 ROWS×COLUMNS 个 int 的有效二维数组；数组原值会被清零。
 * 返回值：无。
 * 异常/错误：C 不使用异常；空指针时直接返回；数组实际尺寸由调用者保证。
 * 副作用：修改 mines，并消耗 rand() 状态；不读写输入输出。
 * 复杂度：N 为格子总数、M 为雷数，时间 O(N+M)，辅助空间 O(N) 存放位置编号。
 * 示例：main 传入刚声明的 mines 数组；函数返回后恰有 10 个不同位置为 1。
 */
static void place_mines(int mines[ROWS][COLUMNS])
{
    int positions[CELL_COUNT];

    if (mines == NULL) {
        return;
    }

    for (int cell = 0; cell < CELL_COUNT; ++cell) {
        positions[cell] = cell;
        mines[cell / COLUMNS][cell % COLUMNS] = 0;
    }

    /*
     * 只洗牌前 M 个位置。每轮从 [index, CELL_COUNT) 选一个，
     * 因而被选位置不会与前面已经固定的雷格重复。
     */
    for (int index = 0; index < MINE_COUNT; ++index) {
        const unsigned int remaining =
            (unsigned int)(CELL_COUNT - index);
        const int swap_index =
            index + (int)random_below(remaining);
        const int temporary = positions[index];

        positions[index] = positions[swap_index];
        positions[swap_index] = temporary;

        const int cell_number = positions[index];
        const int row = cell_number / COLUMNS;
        const int column = cell_number % COLUMNS;
        mines[row][column] = 1;
    }
}

/* ========== 可观察计数：核对本局实际雷数 ========== */

/*
 * 用途：遍历雷图并计算其中非零格子的数量。
 * 参数：mines 指向 ROWS×COLUMNS 个 int 的有效只读二维数组。
 * 返回值：雷格数量，范围为 0..CELL_COUNT；空指针时返回 0。
 * 异常/错误：C 不使用异常；空指针以 0 作为安全默认值处理。
 * 副作用：无，不修改棋盘。
 * 复杂度：N 为格子总数，时间 O(N)，辅助空间 O(1)。
 * 示例：main 在布雷后调用 count_mines(mines)，并打印本局实际布置数量。
 */
static int count_mines(int mines[ROWS][COLUMNS])
{
    int count = 0;

    if (mines == NULL) {
        return 0;
    }

    for (int row = 0; row < ROWS; ++row) {
        for (int column = 0; column < COLUMNS; ++column) {
            count += mines[row][column] != 0;
        }
    }

    return count;
}

/* ========== 相邻雷数与边界：只在坐标有效时访问邻格 ========== */

/*
 * 用途：为每个安全格计算周围 8 个方向的雷数，并重置输出数组。
 * 参数：mines、adjacent 各指向 ROWS×COLUMNS 个 int 的有效二维数组。
 * 返回值：无。
 * 异常/错误：C 不使用异常；任一指针为空时直接返回；实际数组尺寸由调用者保证。
 * 副作用：覆盖 adjacent 的所有元素；只读取 mines。
 * 复杂度：N 为格子总数；每格最多检查 8 个邻格，时间 O(N)，辅助空间 O(1)。
 * 示例：main 在 place_mines 后调用本函数，之后 print_board 读取邻雷数显示数字。
 */
static void calculate_adjacent_counts(
    int mines[ROWS][COLUMNS],
    int adjacent[ROWS][COLUMNS])
{
    if (mines == NULL || adjacent == NULL) {
        return;
    }

    for (int row = 0; row < ROWS; ++row) {
        for (int column = 0; column < COLUMNS; ++column) {
            adjacent[row][column] = 0;
            if (mines[row][column] != 0) {
                continue;
            }

            for (int row_offset = -1; row_offset <= 1; ++row_offset) {
                for (int column_offset = -1;
                     column_offset <= 1;
                     ++column_offset) {
                    const int neighbor_row = row + row_offset;
                    const int neighbor_column = column + column_offset;

                    if (row_offset == 0 && column_offset == 0) {
                        continue;
                    }

                    /*
                     * 先判断行、列都在 [0, 边界) 内，再读取邻格；
                     * 角落自然只会统计 3 格，边缘非角落统计 5 格。
                     */
                    if (neighbor_row < 0 || neighbor_row >= ROWS ||
                        neighbor_column < 0 ||
                        neighbor_column >= COLUMNS) {
                        continue;
                    }

                    if (mines[neighbor_row][neighbor_column] != 0) {
                        ++adjacent[row][column];
                    }
                }
            }
        }
    }
}

/* ========== 安全区域展开：队列保存待展开的空白格 ========== */

/*
 * 用途：揭开一个安全格；若其数字为 0，逐层揭开相邻安全格直到数字边界。
 * 参数：start_row、start_column 是从 0 开始的有效安全格坐标；
 *       mines 和 adjacent 为只读的 ROWS×COLUMNS 棋盘；
 *       revealed 为可写的同尺寸状态数组。
 * 返回值：无。
 * 异常/错误：C 不使用异常；空指针、越界坐标或雷格输入会被忽略，数组真实尺寸由调用者保证。
 * 副作用：把本次可见的安全格在 revealed 中设为 1；不修改雷图和数字图。
 * 复杂度：N 为格子总数；每个格最多处理一次，时间 O(N)，队列辅助空间 O(N)。
 * 示例：main 在确认选中格不是雷后调用本函数；选中 0 格会展开区域，选中数字格只揭开该格。
 */
static void reveal_region(
    int start_row,
    int start_column,
    int mines[ROWS][COLUMNS],
    int adjacent[ROWS][COLUMNS],
    int revealed[ROWS][COLUMNS])
{
    int cell_queue[CELL_COUNT];
    int queue_head = 0;
    int queue_tail = 0;

    if (mines == NULL || adjacent == NULL || revealed == NULL ||
        start_row < 0 || start_row >= ROWS ||
        start_column < 0 || start_column >= COLUMNS ||
        mines[start_row][start_column] != 0) {
        return;
    }

    /* 标记后再入队：从其他空白格再次遇到它时会跳过，不会重复入队。 */
    revealed[start_row][start_column] = 1;
    if (adjacent[start_row][start_column] != 0) {
        return;
    }

    cell_queue[queue_tail] = start_row * COLUMNS + start_column;
    ++queue_tail;

    while (queue_head < queue_tail) {
        const int current_cell = cell_queue[queue_head];
        const int current_row = current_cell / COLUMNS;
        const int current_column = current_cell % COLUMNS;
        ++queue_head;

        for (int row_offset = -1; row_offset <= 1; ++row_offset) {
            for (int column_offset = -1;
                 column_offset <= 1;
                 ++column_offset) {
                const int neighbor_row = current_row + row_offset;
                const int neighbor_column = current_column + column_offset;

                if (row_offset == 0 && column_offset == 0) {
                    continue;
                }
                if (neighbor_row < 0 || neighbor_row >= ROWS ||
                    neighbor_column < 0 ||
                    neighbor_column >= COLUMNS) {
                    continue;
                }
                if (mines[neighbor_row][neighbor_column] != 0 ||
                    revealed[neighbor_row][neighbor_column] != 0) {
                    continue;
                }

                /* 数字边界也揭开，但只把新的 0 格继续放入队列。 */
                revealed[neighbor_row][neighbor_column] = 1;
                if (adjacent[neighbor_row][neighbor_column] == 0) {
                    cell_queue[queue_tail] =
                        neighbor_row * COLUMNS + neighbor_column;
                    ++queue_tail;
                }
            }
        }
    }
}

/* ========== 输入校验：整行读取、解析两个整数并限制坐标范围 ========== */

/*
 * 用途：读取并验证一行“行 列”，把成功坐标转换成从 0 开始的下标。
 * 参数：row、column 是非空 int 输出指针；只在返回 INPUT_OK 时写入。
 * 返回值：INPUT_OK 表示成功；INPUT_END 表示输入结束；
 *         INPUT_INVALID 表示本行格式或范围无效；INPUT_ERROR 表示标准输入错误。
 * 异常/错误：C 不使用异常；用状态值报告错误；strtol 的溢出由 ERANGE 检查。
 * 副作用：从 stdin 读取最多 127 字节；过长行会继续丢弃到换行或 EOF。
 * 复杂度：L 为本行字符数且缓冲区固定为 128 字节，时间 O(L)，辅助空间 O(1)。
 * 示例：main 反复调用本函数；输入“3 5”返回 INPUT_OK 并写入 row=2、column=4。
 */
static int read_coordinates(int *row, int *column)
{
    char line[128];
    char *end = NULL;

    if (row == NULL || column == NULL) {
        return INPUT_INVALID;
    }

    if (fgets(line, sizeof line, stdin) == NULL) {
        return ferror(stdin) ? INPUT_ERROR : INPUT_END;
    }

    /*
     * 缓冲区已满且还没有换行，整行可能被截断。
     * 丢弃本行余下字符，避免它们被误当成下一次坐标输入。
     */
    if (strchr(line, '\n') == NULL && strlen(line) == sizeof line - 1U) {
        int character;

        while ((character = getchar()) != '\n' && character != EOF) {
            /* 仅丢弃过长行剩余字符。 */
        }
        if (ferror(stdin)) {
            return INPUT_ERROR;
        }
        return INPUT_INVALID;
    }

    errno = 0;
    const long input_row = strtol(line, &end, 10);
    if (line == end || errno == ERANGE) {
        return INPUT_INVALID;
    }

    /*
     * 要求两个整数之间至少有一个空白字符，避免把“1+2”误读成两个坐标。
     * 跳过分隔空白后，再由 strtol 解析第二个整数。
     */
    char *separator = end;
    if (!isspace((unsigned char)*separator)) {
        return INPUT_INVALID;
    }
    while (isspace((unsigned char)*separator)) {
        ++separator;
    }

    char *column_start = separator;
    errno = 0;
    const long input_column = strtol(column_start, &end, 10);
    if (column_start == end || errno == ERANGE) {
        return INPUT_INVALID;
    }

    while (isspace((unsigned char)*end)) {
        ++end;
    }
    if (*end != '\0') {
        return INPUT_INVALID;
    }

    if (input_row < 1 || input_row > ROWS ||
        input_column < 1 || input_column > COLUMNS) {
        return INPUT_INVALID;
    }

    *row = (int)(input_row - 1);
    *column = (int)(input_column - 1);
    return INPUT_OK;
}

/* ========== 状态统计与显示：将内部状态映射成玩家可读棋盘 ========== */

/*
 * 用途：统计已经揭开的安全格数，用于判断是否全部 71 格都已找到。
 * 参数：mines、revealed 是只读的 ROWS×COLUMNS 有效数组。
 * 返回值：揭开的安全格数，范围为 0..SAFE_CELL_COUNT；空指针时返回 0。
 * 异常/错误：C 不使用异常；空指针通过 0 作为安全默认值处理。
 * 副作用：无，不修改棋盘。
 * 复杂度：N 为格子总数，时间 O(N)，辅助空间 O(1)。
 * 示例：main 在每次安全揭开后调用本函数，并与 SAFE_CELL_COUNT 比较。
 */
static int count_revealed_safe_cells(
    int mines[ROWS][COLUMNS],
    int revealed[ROWS][COLUMNS])
{
    int count = 0;

    if (mines == NULL || revealed == NULL) {
        return 0;
    }

    for (int row = 0; row < ROWS; ++row) {
        for (int column = 0; column < COLUMNS; ++column) {
            if (mines[row][column] == 0 &&
                revealed[row][column] != 0) {
                ++count;
            }
        }
    }

    return count;
}

/*
 * 用途：显示行列标签、隐藏格、空白、数字；失败时可选择显示全部雷。
 * 参数：mines、adjacent、revealed 为只读的 ROWS×COLUMNS 有效数组；
 *       show_mines 非 0 时额外显示雷格。
 * 返回值：无。
 * 异常/错误：C 不使用异常；空指针时不输出；不检查 printf/putchar 的输出错误。
 * 副作用：向 stdout 写出最多 ROWS×COLUMNS 个格子的可读表示。
 * 复杂度：N 为格子总数，时间 O(N)，辅助空间 O(1)。
 * 示例：main 每回合用 show_mines=0 隐藏雷；踩雷后用 show_mines=1 展示雷图。
 */
static void print_board(
    int mines[ROWS][COLUMNS],
    int adjacent[ROWS][COLUMNS],
    int revealed[ROWS][COLUMNS],
    int show_mines)
{
    if (mines == NULL || adjacent == NULL || revealed == NULL) {
        return;
    }

    printf("\n    ");
    for (int column = 0; column < COLUMNS; ++column) {
        printf("%2d ", column + 1);
    }
    putchar('\n');

    for (int row = 0; row < ROWS; ++row) {
        printf("%2d |", row + 1);
        for (int column = 0; column < COLUMNS; ++column) {
            if (show_mines != 0 && mines[row][column] != 0) {
                printf(" * ");
            } else if (revealed[row][column] == 0) {
                printf(" # ");
            } else if (adjacent[row][column] == 0) {
                printf(" . ");
            } else {
                printf(" %d ", adjacent[row][column]);
            }
        }
        putchar('\n');
    }
}

/* ========== 固定棋盘演示：受控观察角、边和内部格及空白展开 ========== */

/*
 * 用途：在已知布局上观察角落、边缘、内部格的邻雷数和空白区域展开。
 * 参数：无；棋盘由函数内部设置为固定测试布局。
 * 返回值：计数和展开结果符合预期时返回 0，否则返回 1。
 * 异常/错误：C 不使用异常；固定布局不满足预期时以状态码 1 报告。
 * 副作用：向 stdout 显示受控棋盘；只修改局部数组。
 * 复杂度：N 为格子数，时间 O(N)，辅助空间 O(N)，用于三个固定二维数组及展开队列。
 * 示例：main 收到 --algorithm-demo 时调用本函数，不进入随机游戏或读取 stdin。
 */
static int run_algorithm_demo(void)
{
    int demo_mines[ROWS][COLUMNS] = {0};
    int demo_adjacent[ROWS][COLUMNS] = {0};
    int demo_revealed[ROWS][COLUMNS] = {0};

    /* 左上角周围放 3 雷，因此角落相邻格计数应为 3。 */
    demo_mines[0][1] = 1;
    demo_mines[1][0] = 1;
    demo_mines[1][1] = 1;

    /* 顶边第 5 格周围放 5 雷，演示边界处最多只有 5 个邻格。 */
    demo_mines[0][3] = 1;
    demo_mines[0][5] = 1;
    demo_mines[1][3] = 1;
    demo_mines[1][4] = 1;
    demo_mines[1][5] = 1;

    /* 棋盘中央格周围 8 个位置都放雷，展示内部格的完整邻域。 */
    for (int row_offset = -1; row_offset <= 1; ++row_offset) {
        for (int column_offset = -1;
             column_offset <= 1;
             ++column_offset) {
            if (row_offset == 0 && column_offset == 0) {
                continue;
            }
            demo_mines[4 + row_offset][4 + column_offset] = 1;
        }
    }

    calculate_adjacent_counts(demo_mines, demo_adjacent);
    const int corner_count = demo_adjacent[0][0];
    const int edge_count = demo_adjacent[0][4];
    const int interior_count = demo_adjacent[4][4];

    printf("固定布局雷数：%d（普通游戏使用 %d 个雷）\n",
           count_mines(demo_mines), MINE_COUNT);
    printf("邻雷计数：角落=%d（预期 3），边缘=%d（预期 5），内部=%d（预期 8）\n",
           corner_count, edge_count, interior_count);

    /*
     * 右下角远离固定雷群，是一个 0 格。展开后应看见一片空白
     * 以及贴着雷群的数字边界；再次调用不会重复计数。
     */
    puts("从右下角 (9, 9) 展开零区域：");
    reveal_region(8, 8, demo_mines, demo_adjacent, demo_revealed);
    print_board(demo_mines, demo_adjacent, demo_revealed, 0);
    const int opened_after_first = count_revealed_safe_cells(
        demo_mines, demo_revealed);
    reveal_region(8, 8, demo_mines, demo_adjacent, demo_revealed);
    const int opened_after_repeat = count_revealed_safe_cells(
        demo_mines, demo_revealed);
    printf("首次展开后揭开的安全格：%d；重复展开后：%d\n",
           opened_after_first, opened_after_repeat);

    /* 把演示布局中所有安全格设为已揭开，观察通用胜利计数的上限。 */
    for (int row = 0; row < ROWS; ++row) {
        for (int column = 0; column < COLUMNS; ++column) {
            if (demo_mines[row][column] == 0) {
                demo_revealed[row][column] = 1;
            }
        }
    }
    const int all_safe_cells = CELL_COUNT - count_mines(demo_mines);
    const int revealed_safe_cells = count_revealed_safe_cells(
        demo_mines, demo_revealed);
    printf("固定布局的胜利计数：%d / %d\n",
           revealed_safe_cells, all_safe_cells);

    if (corner_count != 3 || edge_count != 5 || interior_count != 8 ||
        opened_after_first == 0 ||
        opened_after_first != opened_after_repeat ||
        revealed_safe_cells != all_safe_cells) {
        return 1;
    }

    return 0;
}

/* ========== 主流程：布雷、输入、揭格并检查输赢 ========== */

/*
 * 用途：初始化并运行完整的随机命令行扫雷游戏。
 * 参数：无；玩家每回合通过标准输入提供两个 1..9 的坐标。
 * 返回值：正常结束、失败或 EOF 停止均返回 0；无法读取时间或输入流错误返回 1。
 * 异常/错误：C 不使用异常；time 失败检查 (time_t)-1，输入状态由 read_coordinates 区分。
 * 副作用：读取当前时间、重置 rand 状态、读 stdin、向 stdout/stderr 输出棋盘和消息。
 * 复杂度：N 为格子数、A 为输入尝试数、B 为输入总字节数，时间 O(A×N+B)；
 *         辅助空间 O(N)，用于 3 个棋盘数组和展开队列。
 * 示例：main 无参数启动本函数；输入一行“3 5”后显示数字、展开空白或在踩雷时结束。
 */
static int run_game(void)
{
    int mines[ROWS][COLUMNS] = {0};
    int adjacent[ROWS][COLUMNS] = {0};
    int revealed[ROWS][COLUMNS] = {0};
    int program_status = 0;

    /*
     * 检查 time 的失败哨兵后才播种。转换到 unsigned int 是标准定义的
     * 模 2^N 转换；同一整数种子会在同一 C 实现中重复其 rand 序列。
     */
    const time_t current_time = time(NULL);
    if (current_time == (time_t)-1) {
        fputs("无法取得当前日历时间，无法播种随机数生成器。\n", stderr);
        return 1;
    }
    const unsigned int game_seed = (unsigned int)current_time;
    show_seed_repeatability(game_seed);
    srand(game_seed);

    place_mines(mines);
    calculate_adjacent_counts(mines, adjacent);
    printf("已布置雷数：%d\n", count_mines(mines));

    puts("扫雷：9 行 9 列，输入“行 列”揭开格子，坐标范围均为 1 到 9。");
    puts("本版本首步也可能踩雷；输入 EOF 可结束游戏。");

    for (;;) {
        int row;
        int column;

        print_board(mines, adjacent, revealed, 0);
        printf("请输入行和列：");
        if (fflush(stdout) == EOF) {
            fputs("无法刷新标准输出。\n", stderr);
            return 1;
        }

        const int input_status = read_coordinates(&row, &column);
        if (input_status == INPUT_END) {
            puts("\n输入结束，游戏停止。");
            break;
        }
        if (input_status == INPUT_ERROR) {
            fputs("\n读取标准输入时发生错误。\n", stderr);
            program_status = 1;
            break;
        }
        if (input_status == INPUT_INVALID) {
            puts("输入无效，请输入两个范围内的整数，例如：3 5。");
            continue;
        }

        if (revealed[row][column] != 0) {
            puts("这个格子已经揭开，请选择其他格子。");
            continue;
        }
        if (mines[row][column] != 0) {
            print_board(mines, adjacent, revealed, 1);
            puts("踩到雷，游戏结束。");
            break;
        }

        reveal_region(row, column, mines, adjacent, revealed);
        print_board(mines, adjacent, revealed, 0);

        if (count_revealed_safe_cells(mines, revealed) ==
            SAFE_CELL_COUNT) {
            puts("所有安全格都已揭开，恭喜获胜！");
            break;
        }
    }

    return program_status;
}


/*
 * 用途：作为 C 程序入口，在普通游戏和受控算法演示之间分派。
 * 参数：argc 是命令行参数个数；argv 保存参数字符串；只接受无参数或单个 --algorithm-demo。
 * 返回值：0 表示所选模式正常结束；1 表示内部演示或运行时错误；2 表示参数用法错误。
 * 异常/错误：C 不使用异常；检查参数个数与模式名，并用状态码报告无效用法。
 * 副作用：根据参数调用 run_game 或 run_algorithm_demo，二者分别执行各自说明的输入输出。
 * 复杂度：分派本身时间与辅助空间均为 O(1)；总复杂度由选中的模式决定。
 * 示例：直接启动进入随机游戏；启动时传入 --algorithm-demo 则显示固定棋盘算法演示。
 */
int main(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1], "--algorithm-demo") == 0) {
        return run_algorithm_demo();
    }
    if (argc != 1) {
        fputs("用法：无参数进入游戏，或使用 --algorithm-demo 查看算法演示。\n",
              stderr);
        return 2;
    }

    return run_game();
}
