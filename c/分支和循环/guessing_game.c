/*
 * 用途：演示 rand/srand 的可复现序列、区间映射、time 播种和猜数字游戏。
 * 输入：游戏中输入 1..100 的整数猜测；无效文本、int 范围外数字或超长行会安全结束。
 * 输出：固定种子是否复现、随机值范围和大小提示。
 * 编译运行：gcc -std=c11 -Wall -Wextra -Wpedantic guessing_game.c -o guessing_game
 */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* SAMPLE_COUNT 的单位为样本个数；游戏区间 GAME_LOW..GAME_HIGH 两端均包含。 */
enum { SAMPLE_COUNT = 3, GAME_LOW = 1, GAME_HIGH = 100 };

/*
 * 用途：观察 rand 的闭区间上界及未显式播种时与 srand(1) 的序列关系。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；rand/srand 无错误返回，标准输出错误未检查。
 * 副作用：消耗并重置 rand 的全局伪随机状态；向 stdout 输出样本与比较结果。
 * 复杂度：时间 O(n)，n 为 SAMPLE_COUNT（当前 3）；辅助空间 O(n)，保存两组样本。
 * 示例：main 首先调用 show_rand_bounds_and_default_seed()，随后调用 show_repeatable_seed()；后者两次用 srand(2026U) 重置序列。
 */
static void show_rand_bounds_and_default_seed(void)
{
    printf("rand() 返回范围：0..RAND_MAX，RAND_MAX=%d\n", RAND_MAX);

    /*
     * 这是 main 中第一次调用 rand：未先调用 srand 的序列应与种子 1 相同。
     */
    /* 两个数组各保存 SAMPLE_COUNT 个 int 样本；rand 每个返回值在 0..RAND_MAX。 */
    int implicit_seed[SAMPLE_COUNT];
    int explicit_seed[SAMPLE_COUNT];
    for (int index = 0; index < SAMPLE_COUNT; ++index) {
        implicit_seed[index] = rand();
    }

    srand(1U);
    for (int index = 0; index < SAMPLE_COUNT; ++index) {
        explicit_seed[index] = rand();
    }

    int same = 1;
    for (int index = 0; index < SAMPLE_COUNT; ++index) {
        if (implicit_seed[index] != explicit_seed[index]) {
            same = 0;
        }
    }
    printf("首次 rand 的样本=%d，位于 0..RAND_MAX：%d\n",
           implicit_seed[0],
           implicit_seed[0] >= 0 && implicit_seed[0] <= RAND_MAX);
    printf("首次 rand 与显式 srand(1) 的序列相同：%s\n",
           same ? "是" : "否");
}

/*
 * 用途：用直接取模把 rand() 映射到闭区间，作为可能有偏差的简化示例。
 * 参数：low、high 为 int 区间端点；要求 low <= high 且区间长度可由 int 表示。
 * 返回值：在 low..high 内的 int。
 * 异常/错误：C 无异常机制；不检查端点，调用方须满足前置条件并避免有符号溢出。
 * 副作用：消耗一次 rand() 伪随机值。
 * 复杂度：时间 O(1)，辅助空间 O(1)；只取一个随机值并计算一次余数。
 * 示例：main 调用 simple_random_in_range(GAME_LOW, GAME_HIGH) 输出普通游戏区间样例。
 */
static int simple_random_in_range(int low, int high)
{
    /*
     * 此教学写法假设 low <= high 且 high-low+1 可由 int 表示；
     * 结果在闭区间内，但当余数不均匀时会有取模偏差。
     */
    return low + rand() % (high - low + 1);
}

/*
 * 用途：比较相同固定种子两次产生的 rand 序列。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；srand/rand 无错误返回，输出错误未检查。
 * 副作用：两次重置并推进 rand 的全局伪随机状态；向 stdout 输出序列。
 * 复杂度：时间 O(n)，n 为 SAMPLE_COUNT（当前 3）；辅助空间 O(n)，保存两组样本。
 * 示例：main 在默认种子示例之后调用 show_repeatable_seed()。
 */
static void show_repeatable_seed(void)
{
    int first_sequence[SAMPLE_COUNT];
    int repeated_sequence[SAMPLE_COUNT];

    srand(2026U);
    for (int index = 0; index < SAMPLE_COUNT; ++index) {
        first_sequence[index] = rand();
    }

    srand(2026U);
    for (int index = 0; index < SAMPLE_COUNT; ++index) {
        repeated_sequence[index] = rand();
    }

    int same = 1;
    for (int index = 0; index < SAMPLE_COUNT; ++index) {
        if (first_sequence[index] != repeated_sequence[index]) {
            same = 0;
        }
    }

    printf("相同固定种子是否复现序列：%s\n", same ? "是" : "否");
    printf("复现的三个值：%d %d %d\n",
           repeated_sequence[0], repeated_sequence[1], repeated_sequence[2]);
}

/*
 * 用途：统计固定来源值 0..9 对 4 取模的频数，展示分组不均匀。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；循环上界和除数固定且有效，输出错误未检查。
 * 副作用：修改局部 residue_counts 并向 stdout 输出四组频数。
 * 复杂度：时间 O(N)，N 为固定来源值个数（当前 10）；辅助空间 O(K)，K 为余数个数（当前 4）。
 * 示例：main 调用 show_modulo_bias_example() 输出 0..9 取模 4 的计数。
 */
static void show_modulo_bias_example(void)
{
    /* residue_counts[i] 统计余数 i 出现次数，i 的范围为 0..3，单位为次。 */
    int residue_counts[4] = {0, 0, 0, 0};

    for (int source_value = 0; source_value < 10; ++source_value) {
        ++residue_counts[source_value % 4];
    }

    printf("固定 0..9 取模 4 的计数：%d、%d、%d、%d\n",
           residue_counts[0],
           residue_counts[1],
           residue_counts[2],
           residue_counts[3]);
}

/*
 * 用途：用拒绝抽样把 rand 值映射到 1..100，避免直接取模的结构性偏差。
 * 参数：无；区间由 GAME_LOW 和 GAME_HIGH 固定。
 * 返回值：闭区间 GAME_LOW..GAME_HIGH 内的游戏目标数。
 * 异常/错误：C 无异常机制；固定区间有效且 RAND_MAX 至少为 32767；拒绝循环没有次数上限。
 * 副作用：每轮抽样消耗 rand() 状态；向 stdout 不输出。
 * 复杂度：期望时间 O(1)（假设 rand 值覆盖其范围）；最坏抽样次数无上界；辅助空间 O(1)。
 * 示例：main 在 srand(time_seed) 后调用 random_game_value() 生成 secret。
 */
static int random_game_value(void)
{
    /*
     * RAND_MAX 至少为 32767，因此这里的抽样范围大于 100。
     * 拒绝末尾不完整的区间块，可避免直接取模产生的结构性偏差。
     * rand 的统计质量仍由实现决定，不能用于安全场景。
     */
    /* source_count、bucket_count、accepted_limit 的单位均为可选整数值个数。 */
    const unsigned long long source_count = (unsigned long long)RAND_MAX + 1ULL;
    const unsigned long long bucket_count =
        (unsigned long long)(GAME_HIGH - GAME_LOW + 1);
    const unsigned long long accepted_limit =
        source_count - source_count % bucket_count;

    unsigned int raw_value;
    do {
        raw_value = (unsigned int)rand();
    } while ((unsigned long long)raw_value >= accepted_limit);

    return GAME_LOW + (int)(raw_value % bucket_count);
}

/*
 * 用途：读取并校验一行十进制文本整数，供猜数循环处理输入。
 * 参数：value 为有效 int 输出指针；成功时写入解析结果。
 * 返回值：1 表示成功，0 表示 EOF，-1 表示无效输入或读取错误。
 * 异常/错误：C 无异常机制；通过返回码、errno、strtol 结束位置和 int 范围检查报告错误；不检查 value 是否为空。
 * 副作用：读取 stdin 并可能丢弃超出固定缓冲区的一行剩余字符；成功时写 *value。
 * 复杂度：时间 O(L)，L 为读取或丢弃的行字节数；辅助空间 O(128) 字节，缓冲区容量固定。
 * 示例：play_game 在每轮猜测时调用 read_integer_line(&guess)，并检查其返回码。
 */
static int read_integer_line(int *value)
{
    /*
     * 先读整行再转换，避免 scanf("%d") 对超出 int 范围的转换触发未定义行为。
     * strtol 的结束位置和 errno 用来发现非数字、尾随杂字符及 long 溢出；
     * 还要显式检查 int 范围。超长行会被丢弃并拒绝，避免残留字符变成下一次输入。
     * 返回 1 表示成功，0 表示输入结束，-1 表示无效输入或读取错误。
     */
    /* line 容量为 128 字节；fgets 每次最多保存 127 个输入字节和结尾 NUL。 */
    char line[128];
    if (fgets(line, sizeof line, stdin) == NULL) {
        return ferror(stdin) ? -1 : 0;
    }

    size_t length = 0;
    while (line[length] != '\0') {
        ++length;
    }

    /* 缓冲区内没有换行表示输入行未完整装入；丢弃剩余部分并拒绝该行。 */
    if (length > 0 && line[length - 1] != '\n') {
        int character = getchar();
        if (character != EOF) {
            while (character != '\n' && character != EOF) {
                character = getchar();
            }
            return -1;
        }
        if (ferror(stdin)) {
            return -1;
        }
    }

    errno = 0;
    char *end = NULL;
    long parsed = strtol(line, &end, 10);
    if (end == line) {
        return -1;
    }

    while (isspace((unsigned char)*end)) {
        ++end;
    }
    if (*end != '\0' || errno == ERANGE || parsed < INT_MIN || parsed > INT_MAX) {
        return -1;
    }

    *value = (int)parsed;
    return 1;
}

/*
 * 用途：循环读取猜测并与目标比较，直到猜中、输入结束或输入无效。
 * 参数：secret 为目标整数；调用方应传入 GAME_LOW..GAME_HIGH 内的值。
 * 返回值：0 表示猜中；1 表示 EOF、读取错误或无效输入导致结束。
 * 异常/错误：C 无异常机制；输入错误由 read_integer_line 的 0/-1 返回码识别；不校验 secret 范围。
 * 副作用：提示并读取 stdin，向 stdout 输出反馈；调用 read_integer_line。
 * 复杂度：时间 O(G + B)，G 为猜测轮数，B 为各行读取/丢弃的总字节数；辅助空间 O(1)（行缓冲区固定）。
 * 示例：main 将 random_game_value() 的结果传给 play_game(secret)。
 */
static int play_game(int secret)
{
    puts("请输入 1 到 100 的整数；猜中后结束。"
         "超长行、非整数和 int 范围外输入会安全结束。");

    for (;;) {
        /* guess 是用户输入的整数猜测；接受范围为 1..100，单位为猜测值。 */
        int guess;
        printf("你的猜测：");
        fflush(stdout);

        int input_status = read_integer_line(&guess);
        if (input_status <= 0) {
            puts(input_status == 0
                ? "输入结束，游戏结束。"
                : "输入不是有效 int 整数，游戏结束。");
            return 1;
        }

        /* 越界猜测不进入大小比较；提示后继续读取下一整行。 */
        if (guess < GAME_LOW || guess > GAME_HIGH) {
            puts("数字必须在 1 到 100 之间。");
            continue;
        }

        if (guess < secret) {
            puts("再大些。");
        } else if (guess > secret) {
            puts("再小些。");
        } else {
            puts("猜对了！");
            return 0;
        }
    }
}

/*
 * 用途：把有效 time_t 值转换为 unsigned int 播种值，并拒绝 time 的失败哨兵。
 * 参数：current_time 为 time(NULL) 结果；seed 为有效 unsigned int 输出指针。
 * 返回值：1 表示已写入种子，0 表示 current_time 等于失败哨兵。
 * 异常/错误：C 无异常机制；通过返回值报告失败；不检查 seed 是否为空。
 * 副作用：成功时写入 *seed；不调用时间 API 或修改随机数状态。
 * 复杂度：时间 O(1)，辅助空间 O(1)；只比较并转换一个值。
 * 示例：main 对 time(NULL) 结果调用 make_seed_from_time，并另用 (time_t)-1 验证拒绝路径。
 */
static int make_seed_from_time(time_t current_time, unsigned int *seed)
{
    /* time 失败时返回 (time_t)-1，先识别哨兵，避免把失败值当作种子。 */
    if (current_time == (time_t)-1) {
        return 0;
    }

    /* 常见平台的 time_t 当前值可转换为 unsigned int；转换会折叠到种子范围。 */
    *seed = (unsigned int)current_time;
    return 1;
}

/*
 * 用途：运行随机数演示、生成游戏目标并启动猜数字交互。
 * 参数：无。
 * 返回值：1 表示读取当前时间失败；否则返回 play_game 的 0/1 结果。
 * 异常/错误：C 无异常机制；time 失败由 (time_t)-1 检查，输入错误由 play_game 的返回值报告。
 * 副作用：读当前时间与 stdin，修改 rand 全局状态，向 stdout/stderr 输出；依次调用各演示和游戏函数。
 * 复杂度：时间 O(G + B) 加拒绝抽样时间；G 为猜测轮数，B 为输入总字节数，随机抽样最坏无界；辅助空间 O(1)。
 * 示例：运行本文件进入 main(void)，完成随机数说明后输入 1..100 的整数参与游戏。
 */
int main(void)
{
    show_rand_bounds_and_default_seed();
    show_repeatable_seed();
    show_modulo_bias_example();

    /* current_time 保存 time_t 日历时间；time(NULL) 返回 (time_t)-1 表示失败。 */
    time_t current_time = time(NULL);
    /* time_seed 是实现相关位宽的无符号种子；相同种子会重现同一实现的 rand 序列。 */
    unsigned int time_seed;
    if (!make_seed_from_time(current_time, &time_seed)) {
        fputs("无法读取当前日历时间。\n", stderr);
        return 1;
    }
    /* 本例的 sizeof(time_t) 可由 unsigned long 表示，显式转换后使用匹配的 %lu。 */
    printf("当前实现 sizeof(time_t)=%lu bytes\n",
           (unsigned long)sizeof current_time);

    /* 直接构造标准失败哨兵，观察转换函数拒绝它而不触发真实系统故障。 */
    unsigned int ignored_seed = 0U;
    int failure_sentinel_rejected =
        !make_seed_from_time((time_t)-1, &ignored_seed);
    printf("构造的 time 失败哨兵被拒绝：%d\n", failure_sentinel_rejected);

    /* 种子位数有限，同一整数种子会复现序列，短时间内也可能重复。 */
    printf("用于普通游戏的 unsigned int 种子：%u\n", time_seed);
    srand(time_seed);

    int simple_sample = simple_random_in_range(GAME_LOW, GAME_HIGH);
    printf("直接取模映射示例（范围内，但可能有偏差）：%d\n",
           simple_sample);

    int secret = random_game_value();
    return play_game(secret);
}
