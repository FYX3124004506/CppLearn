/*
 * 演示常见类型、对象大小、数值范围和运算符。
 * 输入：无。使用 C11 编译后运行。
 * 普通标识符区分大小写，以字母或下划线开头，后续可含字母、数字和下划线；关键字保留。
 * 作用域和存储期由语言规则定义；对象的具体内存位置由实现决定。
 */
#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* 整数类型和浮点类型属于算术类型；指针类型属于标量类型。 */
/* 数组和结构是聚合类型；联合属于另一类类型。 */
/* _Bool 和枚举类型属于整数类型。 */
struct Point { int x; int y; };
union Number { int integer; float real; };
enum Status { STATUS_IDLE, STATUS_READY = 3, STATUS_DONE };

int file_scope_count; /* 暂定定义具有静态存储期；未显式初始化时会被零初始化。 */

static int add(int left, int right)
{
    return left + right;
}

static void section(const char *title)
{
    printf("\n-- %s --\n", title);
}

#if !defined(__STDC_NO_VLA__)
static void show_vla_size(void)
{
    size_t length = 3;
    int values[length];
    for (size_t i = 0; i < length; ++i) {
        values[i] = (int)i + 1;
    }
    printf("VLA: %zu elements, %zu bytes, last=%d\n",
           length, sizeof values, values[length - 1]);
}
#else
static void show_vla_size(void)
{
    puts("Variable length arrays are not supported by this implementation.");
}
#endif

int main(void)
{
    extern int file_scope_count; /* 声明已经存在的文件作用域对象。 */
    section("character and integer types");
    char letter = 'A'; /* 字符常量的数值由实现的执行字符集决定。 */
    signed char negative_byte = -10;
    unsigned char byte = UCHAR_MAX;
    short short_value = -12;
    unsigned short unsigned_short_value = 12U;
    int integer_value = -25;
    unsigned int unsigned_value = 25U;
    long long_value = 123456L;
    unsigned long unsigned_long_value = 123456UL;
    long long long_long_value = 9876543210LL;
    unsigned long long unsigned_long_long_value = 9876543210ULL;
    int character_constant_is_int = _Generic('A', int: 1, default: 0);

    printf("char=%c; plain char is %s; 'A' has int type=%s\n",
           letter, CHAR_MIN < 0 ? "signed" : "unsigned",
           character_constant_is_int ? "yes" : "no");
    printf("signed char=%hhd; unsigned char max=%hhu\n", negative_byte, byte);
    printf("short=%hd, unsigned short=%hu, int=%d, unsigned=%u\n",
           short_value, unsigned_short_value, integer_value, unsigned_value);
    printf("long=%ld/%lu; long long=%lld/%llu\n",
           long_value, unsigned_long_value, long_long_value,
           unsigned_long_long_value);
    printf("CHAR_BIT=%d; int=[%d,%d], UINT_MAX=%u\n",
           CHAR_BIT, INT_MIN, INT_MAX, UINT_MAX);
    printf("short=[%hd,%hd], long=[%ld,%ld], long long=[%lld,%lld]\n",
           SHRT_MIN, SHRT_MAX, LONG_MIN, LONG_MAX, LLONG_MIN, LLONG_MAX);
    if (UCHAR_MAX < UINT_MAX) {
        unsigned int one_past_char = (unsigned int)UCHAR_MAX + 1U;
        unsigned char narrowed = (unsigned char)one_past_char;
        printf("unsigned conversion: %u -> %u\n",
               one_past_char, (unsigned)narrowed);
    }

    int_least32_t at_least_32 = 123;
    printf("minimum-width type: %" PRIdLEAST32 "\n", at_least_32);
#ifdef UINT32_MAX
    uint32_t exact_32 = UINT32_C(42);
    printf("exact-width uint32_t available: %" PRIu32 "\n", exact_32);
#else
    puts("No exact 32-bit unsigned type is provided.");
#endif

    section("floating point, boolean, and enumeration");
    float single = 0.75f;
    double regular = 12.5;
    long double extended = 1.0L / 3.0L;
    _Bool normalized = 99;
    bool ready = true;
    enum Status status = STATUS_DONE;

    printf("float=%.2f, double=%.2f, long double=%.4Lf\n",
           single, regular, extended);
    printf("_Bool normalizes nonzero to %d; bool=%s\n",
           normalized, ready ? "true" : "false");
    printf("enum status=%d; STATUS_DONE is the next implicit value\n", (int)status);
    const char *enum_compatible_type = _Generic(
        status,
        char: "char", signed char: "signed char", unsigned char: "unsigned char",
        short: "short", unsigned short: "unsigned short",
        int: "int", unsigned int: "unsigned int",
        long: "long", unsigned long: "unsigned long",
        long long: "long long", unsigned long long: "unsigned long long",
        default: "other compatible integer type");
    printf("enum compatible integer type on this implementation=%s\n",
           enum_compatible_type);
    printf("FLT_DIG=%d, DBL_DIG=%d, LDBL_DIG=%d, FLT_MAX=%g\n",
           FLT_DIG, DBL_DIG, LDBL_DIG, (double)FLT_MAX);
    double decimal_sum = 0.1 + 0.2;
    printf("0.1 + 0.2 = %.17g; equal to 0.3: %s\n", decimal_sum,
           decimal_sum == 0.3 ? "yes" : "no");
    /* 显示的舍入结果和比较结果可能随实现而异。 */
    /* 枚举类型所兼容的整数类型由实现定义。 */

    section("arrays, structures, unions, pointers, and functions");
    int values[] = {2, 3, 5};
    struct Point point = {3, 4};
    union Number number = {.integer = 42};
    int (*operation)(int, int) = add;
    /* 函数指针与对象指针不同；这里只把对象指针转换后交给 %p 输出。 */
    int *first = values;
    void *object_pointer = &point;

    printf("array=%d, struct=(%d,%d), union=%d, add(2,3)=%d\n",
           *first, point.x, point.y, number.integer, operation(2, 3));
    printf("object pointer=%p\n", (void *)object_pointer);
    /* void 指针可以保存对象指针，但不能直接解引用。 */

    section("sizeof and object sizes");
    int probe = 4;
    size_t expression_size = sizeof(probe++);
    /* 标准整数类型的大小按等级不递减；具体大小由实现决定。 */
    printf("char=%zu, short=%zu, int=%zu, long=%zu, long long=%zu bytes\n",
           sizeof(char), sizeof(short), sizeof(int), sizeof(long),
           sizeof(long long));
    printf("signed char=%zu, unsigned char=%zu bytes\n",
           sizeof(signed char), sizeof(unsigned char));
    printf("float=%zu, double=%zu, long double=%zu bytes\n",
           sizeof(float), sizeof(double), sizeof(long double));
    printf("array=%zu, struct=%zu, union=%zu, pointer=%zu bytes\n",
           sizeof values, sizeof point, sizeof number, sizeof first);
    printf("sizeof(probe++)=%zu; probe remains %d; sizeof *first=%zu\n",
           expression_size, probe, sizeof *first);
    show_vla_size();

    section("arithmetic and conversions");
    int difference = 9 - 4;
    int product = 3 * 4;
    int integer_quotient = 6 / 4;
    int negative_quotient = -6 / 4;
    double floating_quotient = 6 / 4.0;
    int remainder = -13 % 5;
    int negative = -1;
    unsigned int mixed = negative + 1U;
    unsigned int wrapped = UINT_MAX;

    wrapped += 1U;
    printf("9-4=%d, 3*4=%d; 6/4=%d, -6/4=%d, 6/4.0=%.1f, -13%%5=%d\n",
           difference, product, integer_quotient, negative_quotient,
           floating_quotient, remainder);
    printf("mixed signed/unsigned=%u; UINT_MAX+1=%u\n", mixed, wrapped);
/* 严格小于时提升后的 int 才能安全计算 UCHAR_MAX + 1；相等时改用 unsigned int。 */
#if UCHAR_MAX < INT_MAX
    int promoted_sum = (unsigned char)UCHAR_MAX + 1;
    printf("unsigned char promotes to int: UCHAR_MAX+1=%d\n", promoted_sum);
#else
    unsigned int promoted_sum = (unsigned char)UCHAR_MAX + 1U;
    printf("unsigned char promotes to unsigned int: %u\n", promoted_sum);
#endif
    int denominators[] = {0, 4};
    for (size_t i = 0; i < sizeof denominators / sizeof denominators[0]; ++i) {
        int denominator = denominators[i];
        if (denominator == 0) {
            puts("division and remainder skipped for zero denominator");
        } else {
            printf("guarded division/remainder: 20/%d=%d, 20%%%d=%d\n",
                   denominator, 20 / denominator, denominator, 20 % denominator);
        }
    }
    /* 有符号运算超出可表示范围时行为未定义；此程序不执行越界运算。 */
    if (INT_MIN < -INT_MAX) {
        puts("本实现的 INT_MIN / -1 商无法用 int 表示；仅检查边界，不执行除法。");
    } else {
        puts("本实现的 int 范围可以表示 INT_MIN / -1 的数学商。");
    }

    section("assignment and increment");
    int a, b, c;
    a = b = c = 10;
    int count = 5;
    count += 3;
    count *= 2;
    /* 其他复合赋值形式包括 -=、/=、%=、<<=、>>=、&=、^= 和 |=。 */
    int old_value = count++;
    int new_value = ++count;
    int decrement = 3;
    int old_decrement = decrement--;
    int new_decrement = --decrement;
    int indexed[] = {1, 2, 3};
    size_t index = 0;
    indexed[index++] += 5;
    printf("right-associative assignment: %d,%d,%d\n", a, b, c);
    printf("compound=%d; ++ postfix=%d, prefix=%d; -- postfix=%d, prefix=%d\n",
           count, old_value, new_value, old_decrement, new_decrement);
    printf("compound left side evaluated once: index=%zu\n", index);
    printf("updated element=%d; unary plus=%d, unary minus=%d\n",
           indexed[0], +count, -count);
    /* 在一次调用中无序地多次修改同一标量对象会导致未定义行为。 */

    section("explicit conversion");
    double decimal = -3.14;
    int whole = (int)decimal;
    printf("float-to-int truncates toward zero: %.2f -> %d\n", decimal, whole);
    /* 浮点值超出目标整数类型范围时，不要执行这种转换。 */
    printf("static-storage file object starts at %d\n", file_scope_count);
    return 0;
}
