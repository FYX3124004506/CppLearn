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

enum { SAMPLE_COUNT = 3, GAME_LOW = 1, GAME_HIGH = 100 };

static void show_rand_bounds_and_default_seed(void)
{
    printf("rand() 返回范围：0..RAND_MAX，RAND_MAX=%d\n", RAND_MAX);

    /*
     * 这是 main 中第一次调用 rand：未先调用 srand 的序列应与种子 1 相同。
     */
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

static int simple_random_in_range(int low, int high)
{
    /*
     * 此教学写法假设 low <= high 且 high-low+1 可由 int 表示；
     * 结果在闭区间内，但当余数不均匀时会有取模偏差。
     */
    return low + rand() % (high - low + 1);
}

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

static void show_modulo_bias_example(void)
{
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

static int random_game_value(void)
{
    /*
     * RAND_MAX 至少为 32767，因此这里的抽样范围大于 100。
     * 拒绝末尾不完整的区间块，可避免直接取模产生的结构性偏差。
     * rand 的统计质量仍由实现决定，不能用于安全场景。
     */
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

static int read_integer_line(int *value)
{
    /*
     * 先读整行再转换，避免 scanf("%d") 对超出 int 范围的转换触发未定义行为。
     * strtol 的结束位置和 errno 用来发现非数字、尾随杂字符及 long 溢出；
     * 还要显式检查 int 范围。超长行会被丢弃并拒绝，避免残留字符变成下一次输入。
     * 返回 1 表示成功，0 表示输入结束，-1 表示无效输入或读取错误。
     */
    char line[128];
    if (fgets(line, sizeof line, stdin) == NULL) {
        return ferror(stdin) ? -1 : 0;
    }

    size_t length = 0;
    while (line[length] != '\0') {
        ++length;
    }

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

static int play_game(int secret)
{
    puts("请输入 1 到 100 的整数；猜中后结束。"
         "超长行、非整数和 int 范围外输入会安全结束。");

    for (;;) {
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

int main(void)
{
    show_rand_bounds_and_default_seed();
    show_repeatable_seed();
    show_modulo_bias_example();

    time_t current_time = time(NULL);
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
