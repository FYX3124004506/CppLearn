/*
 * 演示 scanf 格式规则，以及基于整行文本的安全数值解析。
 * 输入三行：姓名、非负整数、非负有限数。
 * 使用 C11 编译并运行，然后按提示输入这三项内容。
 */
/*
 * 文件用途：示范 scanf 家族的格式规则，并按整行读取、校验和解析交互输入。
 * 输入：先运行内置 sscanf 演示，再从标准输入依次读取姓名、非负整数和非负有限数。
 * 输出：演示结果及最终字段写到标准输出；无效输入说明写到标准错误，并返回失败状态。
 */
#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* scanf 的 %f 接收 float *，%lf 接收 double *；printf 的 %f 对应 double 实参。 */
/*
 * 用途：用固定字符串调用 sscanf，展示赋值计数、空白、字段宽度和转换抑制。
 * 参数：无；被扫描的示例字符串在函数内定义，不读取 stdin。
 * 返回值：无（void）。各次 sscanf 的 int 结果用于显示匹配数/EOF；个别已知有效的示例故意忽略结果。
 * 异常/错误：C 不使用异常机制；固定格式和目标类型配对，%s 均设宽度；示例把匹配失败作为返回值展示，没有异常抛出。
 * 副作用：向标准输出写入演示结果；只写入本函数的局部变量，不改变调用者状态。
 * 复杂度：时间 O(N)，N 为固定示例文本的字节数；辅助空间 O(1)，缓冲区大小固定（word 为 20 字节）。
 * 示例：main 在提示用户输入前调用 demonstrate_scanf_formats()；函数内部实际调用 sscanf(sample, ...) 和多个对照格式。
 */
static void demonstrate_scanf_formats(void)
{
    const char sample[] = "-12 42 3.5 6.25 Z word";
    int signed_value = 0;
    unsigned int unsigned_value = 0;
    float single_value = 0.0f;
    double double_value = 0.0;
    char character = '\0';
    char word[20] = "";
/*
 * 字段宽度说明：%19s 为 word 留出末尾空字符，因此最多写入 19 个字节；sscanf 返回成功赋值的项目数。
 */
    int assigned = sscanf(sample, "%d %u %f %lf %c %19s",
                          &signed_value, &unsigned_value, &single_value,
                          &double_value, &character, word);

    printf("assigned=%d: int=%d unsigned=%u float=%.1f double=%.2f char=%c word=%s\n",
           assigned, signed_value, unsigned_value, single_value,
           double_value, character, word);

    char raw_character = '\0';
    char first_nonspace = '\0';
/*
 * 空白说明：%c 不跳过前导空白，所以第一次读取到空格；格式串中的前导空格会跳过任意空白后再读字符。
 */
    int raw_result = sscanf(" A", "%c", &raw_character);
    int spaced_result = sscanf(" A", " %c", &first_nonspace);
    printf("%%c without leading space reads whitespace=%s; with space reads=%c\n",
           raw_result == 1 && raw_character == ' ' ? "yes" : "no",
           spaced_result == 1 ? first_nonspace : '?');

    int left = 0;
    int right = 0;
    int partial = sscanf("17 nope", "%d %d", &left, &right);
    printf("matching failure after one assignment returns %d\n", partial);

    int year = 0;
    int suppressed = sscanf("2026-10", "%d-%*d", &year);
    printf("suppressed conversion returns %d assignment; year=%d\n", suppressed, year);

    int empty_value = 0;
    int empty_result = sscanf("", "%d", &empty_value);
    printf("empty input before conversion returns EOF=%s\n",
           empty_result == EOF ? "yes" : "no");

    char first_word[20] = "";
    (void)sscanf("red blue", "%19s", first_word);
    printf("bounded %%s stops at whitespace: %s\n", first_word);

/*
 * 边界说明：%2s 限制的是窄字符输入字节数而非 Unicode 字符数；两字节字段可能截断 UTF-8 字符，所以这里只统计字节。
 */
    /* 只统计固定 UTF-8 字节序列的输入长度，不把可能截断的字节序列作为文字输出。 */
#if CHAR_BIT == 8
    const unsigned char utf8_bytes[] = {0xE4U, 0xB8U, 0xADU, 0x41U, 0U};
    char utf8_source[sizeof utf8_bytes];
    char first_two_bytes[3] = "";
    memcpy(utf8_source, utf8_bytes, sizeof utf8_bytes);
    int utf8_assigned = sscanf(utf8_source, "%2s", first_two_bytes);
    printf("UTF-8 sample input bytes=%zu; field width=2 stored bytes=%zu; assigned=%d\n",
           sizeof utf8_source - 1, strlen(first_two_bytes), utf8_assigned);
#else
    puts("UTF-8 byte-count example skipped because this target does not use 8-bit bytes.");
#endif
    /* 窄字符 %s 的字段宽度按输入字节限制；多字节文本要按缓冲区字节容量设置宽度。 */
    /* scanf 的多数转换会跳过前导空白；%c、%[ 和 %n 不会。 */
    /* scanf_s 属于可选的附录 K 接口；数值输入超出目标类型范围时不能依赖 scanf 安全处理。 */
}

/*
 * 用途：从 stdin 读取一行文本，去掉行尾换行；拒绝容量不足、读失败或超长行。
 * 参数：buffer 指向至少 capacity 字节的可写缓冲区；capacity 的单位是字节，必须至少为 2。
 * 返回值：int；1 表示成功，0 表示参数容量不合要求、EOF/读取失败或行过长；成功时 buffer 以空字符结尾。
 * 异常/错误：C 不使用异常机制；用 fgets/getchar 的结果和 0/1 返回值报告错误；调用者须提供有效缓冲区。
 * 副作用：消耗 stdin 中当前行；若行过长，会继续读取并丢弃该行剩余内容；成功时改写 buffer。
 * 复杂度：时间 O(L)，L 为本行读取或丢弃的字节数；辅助空间 O(1)，不分配新内存。
 * 示例：main 用 read_line 读姓名；其余两行也分别传入 64 字节数组。
 */
/*
 * 读取边界：fgets 最多写入 capacity-1 个输入字节并补空字符；若缓冲区内没有换行，还要检查并在超长时排空本行剩余字节。
 */
static int read_line(char *buffer, int capacity)
{
    if (capacity < 2 || fgets(buffer, capacity, stdin) == NULL) {
        return 0;
    }

/*
 * 长度单位：length 按字节计数，不是多字节编码文本的字符数；本辅助函数面向普通文本行，不把内嵌 NUL 的二进制数据作为输入格式。
 */
    size_t length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[--length] = '\0';
    } else {
        int ch = getchar();
        if (ch != '\n' && ch != EOF) {
            while (ch != '\n' && ch != EOF) {
                ch = getchar();
            }
            return 0;
        }
    }
    if (length > 0 && buffer[length - 1] == '\r') {
        buffer[--length] = '\0';
    }
    return 1;
}

/*
 * 用途：把一整行十进制文本解析为 int，并拒绝溢出、空输入和非空白尾随字符。
 * 参数：text 是以空字符结尾的输入文本；result 指向可写 int，两个指针都必须有效。
 * 返回值：int；1 表示成功且已写入 *result，0 表示解析失败；允许前导/尾随空白和 strtol 接受的正负号。
 * 异常/错误：C 不使用异常机制；通过 strtol 的 end 指针、errno、int 范围检查及 0/1 返回值处理错误。
 * 副作用：先把 errno 置零；转换可能再次设置 errno；仅成功时写入 *result，不会部分写入结果。
 * 复杂度：时间 O(N)，N 为文本字节数（包含尾部空白扫描）；辅助空间 O(1)，无动态分配。
 * 示例：main 调用 parse_int_line(integer_text, &age)，再单独检查 age 是否非负。
 */
/*
 * 解析边界：end 用来定位数字转换停止处；允许其后的空白，但任何其他尾随字符都会使整行无效。
 */
static int parse_int_line(const char *text, int *result)
{
    char *end;
    errno = 0;
    long value = strtol(text, &end, 10);

    if (errno == ERANGE || end == text) {
        return 0;
    }
    while (isspace((unsigned char)*end)) {
        ++end;
    }
    if (*end != '\0' || value < INT_MIN || value > INT_MAX) {
        return 0;
    }
    *result = (int)value;
    return 1;
}

/*
 * 用途：解析一整行 double，并只接受有限且不小于 0 的值。
 * 参数：text 是以空字符结尾的文本；result 指向可写 double；调用者须保证指针有效。
 * 返回值：int；1 表示成功并写入结果，0 表示范围错误、没有数字、尾随非空白字符、非有限值或负值。
 * 异常/错误：C 不使用异常机制；用 strtod 的 end 指针/errno、isfinite、范围条件和 0/1 状态报告问题。
 * 副作用：先将 errno 置零，转换可能改变 errno；只在成功时写入 *result。
 * 复杂度：时间 O(N)，N 为输入文本字节数；辅助空间 O(1)，不创建额外缓冲区。
 * 示例：main 调用 parse_finite_nonnegative_double(decimal_text, &measurement) 解析第三行输入。
 */
/*
 * 解析边界：除完整消费数字（尾随空白除外）外，还要拒绝 NaN、正负无穷和负数，保证后续展示值有限且非负。
 */
static int parse_finite_nonnegative_double(const char *text, double *result)
{
    char *end;
    errno = 0;
    double value = strtod(text, &end);

    if (errno == ERANGE || end == text) {
        return 0;
    }
    while (isspace((unsigned char)*end)) {
        ++end;
    }
    if (*end != '\0' || !isfinite(value) || value < 0.0) {
        return 0;
    }
    *result = value;
    return 1;
}

/*
 * 用途：先展示 sscanf 规则，再读取、校验并回显姓名、年龄和测量值。
 * 参数：无；通过 stdin 交互读取三行输入。
 * 返回值：int；成功返回 0；任何一行读取/校验失败返回 1。
 * 异常/错误：C 不使用异常机制；依次检查 read_line 和解析函数的状态；年龄额外要求非负，测量值要求有限且非负。
 * 副作用：向 stdout 输出提示和结果，错误信息写入 stderr；读取并消耗三行 stdin。
 * 复杂度：时间 O(N)，N 为三行文本的总字节数，另加固定 sscanf 演示；辅助空间 O(1)，三个输入数组各 64 字节。
 * 示例：运行程序后按提示输入姓名、非负整数和非负有限数；main 调用辅助函数并打印下一年龄（INT_MAX 时改为提示）。
 */
int main(void)
{
/*
 * 缓冲区单位与范围：三个数组各有 64 字节，最多容纳 63 个输入字节和结尾空字符；UTF-8 等多字节姓名的可容纳字符数可能更少。
 */
    char name[64];
    char integer_text[64];
    char decimal_text[64];
    int age;
    double measurement;

    demonstrate_scanf_formats();

    printf("Name (one line): ");
    if (!read_line(name, (int)sizeof name) || name[0] == '\0') {
        fputs("Name is empty or too long.\n", stderr);
        return 1;
    }

    printf("Age (nonnegative integer): ");
    if (!read_line(integer_text, (int)sizeof integer_text) ||
        !parse_int_line(integer_text, &age) || age < 0) {
        fputs("Age is invalid or outside the int range.\n", stderr);
        return 1;
    }

    printf("Measurement (finite, nonnegative number): ");
    if (!read_line(decimal_text, (int)sizeof decimal_text) ||
        !parse_finite_nonnegative_double(decimal_text, &measurement)) {
        fputs("Measurement is invalid or outside the double range.\n", stderr);
        return 1;
    }

    printf("Name: %s\nAge: %d\nMeasurement: %.2f\n", name, age, measurement);
/*
 * 溢出保护：int 最大值再加 1 不可表示，因此先比较 INT_MAX；只有较小的 age 才执行 age + 1。
 */
    if (age == INT_MAX) {
        puts("Next age is outside the int range.");
    } else {
        printf("Next age: %d\n", age + 1);
    }
    return 0;
}
