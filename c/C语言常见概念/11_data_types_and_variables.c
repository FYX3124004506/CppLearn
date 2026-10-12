/*
 * 演示常见类型、对象大小、数值范围和运算符。
 * 输入：无。使用 C11 编译后运行。
 * 普通标识符区分大小写，以字母或下划线开头，后续可含字母、数字和下划线；关键字保留。
 * 作用域和存储期由语言规则定义；对象的具体内存位置由实现决定。
 */
/*
 * 文件用途：集中演示 C 的基本/复合类型、对象大小、数值范围、转换和运算符。
 * 输入：无；所有数据均在程序内部构造，条件分支由编译环境决定。
 * 输出：标准输出显示类型、范围、大小及运算示例；个别结果注明可能随实现变化。
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

/*
 * 变量说明：file_scope_count 是文件作用域 int 示例对象；假设/待确认：其单位和业务含义未提供，本文件只读取零初始化后的值。
 */
int file_scope_count; /* 暂定定义具有静态存储期；未显式初始化时会被零初始化。 */

/*
 * 用途：返回两个 int 的和，供函数指针和直接调用示例使用。
 * 参数：left、right 是 int；调用者应保证数学和在 int 可表示范围内。
 * 返回值：int；返回 left + right。
 * 异常/错误：C 不使用异常机制；不检查有符号溢出，越界相加会导致未定义行为，调用者负责保证范围。
 * 副作用：无；只计算并返回。
 * 复杂度：时间 O(1)，辅助空间 O(1)；一次整数加法。
 * 示例：main 将 add 赋给 operation，再调用 operation(2, 3)；该调用实际返回 5。
 */
static int add(int left, int right)
{
    return left + right;
}

/*
 * 用途：打印带空行和标题的分节标记，便于区分 main 的输出主题。
 * 参数：title 是指向有效、以空字符结尾字符串的指针；本函数不复制它。
 * 返回值：无（void）。
 * 异常/错误：C 不使用异常机制；不检查 title 是否为空指针或有效字符串，调用者须提供有效值；printf 错误未检查。
 * 副作用：向标准输出写入一个空行、标题和换行。
 * 复杂度：时间 O(N)，N 为标题字符串长度；辅助空间 O(1)。
 * 示例：main 多次调用 section 打印各演示主题的小节标题。
 */
static void section(const char *title)
{
    printf("\n-- %s --\n", title);
}

/*
 * 用途：按实现是否支持可变长度数组，提供同名 show_vla_size 的对应版本。
 * 参数：无；数组长度在支持 VLA 的版本中固定为 3。
 * 返回值：无（void）。
 * 异常/错误：C 不使用异常机制；这里不读取输入，也不返回错误状态；不支持 VLA 时改为打印说明。
 * 副作用：向标准输出打印数组元素数、字节数和末元素，或打印不支持说明。
 * 复杂度：时间 O(N)，N 为数组元素数且本例 N=3；辅助空间 O(N) 用于 VLA（本例固定 3 个 int），不支持时为 O(1)。
 * 示例：main 在 sizeof 小节调用 show_vla_size()；预处理条件决定执行哪个定义。
 */
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

/*
 * 用途：依次输出类型/变量、复合类型、大小、算术、赋值及显式转换示例。
 * 参数：无；本程序不读取命令行或标准输入。
 * 返回值：int；返回 0 表示正常结束。
 * 异常/错误：C 不使用异常机制；代码用条件分支避开除以零和不可表示的 INT_MIN/-1 商，并避免有符号越界；输出错误不检查。
 * 副作用：向标准输出写入多节信息，并更新若干局部变量；文件作用域 file_scope_count 只读取、不修改。
 * 复杂度：时间 O(1)，辅助空间 O(1)；数组长度、循环次数和 VLA 长度均是固定常数，空间计入固定局部对象。
 * 示例：运行程序后 main 实际调用 section、add、show_vla_size 和多个 printf；函数指针 operation 也会调用 add。
 */
int main(void)
{
    extern int file_scope_count; /* 声明已经存在的文件作用域对象。 */
    section("character and integer types");
/*
 * 取值说明：letter 保存一个 char 字符；signed char 示例为 -10，unsigned char 示例取 UCHAR_MAX，范围由实现的 limits.h 常量给出。
 */
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
/*
 * 转换边界：只有 UCHAR_MAX+1 能由 unsigned int 表示时才计算该值；转回 unsigned char 按该类型的模范围转换。
 */
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
/*
 * 关键变量：single、regular、extended 分别演示三种浮点类型；值为无单位示例，精度和范围由实现决定。
 */
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
/*
 * 浮点说明：十进制小数通常不能被二进制浮点精确表示；本例打印实际近似值并直接比较，不预设所有实现结果相同。
 */
    double decimal_sum = 0.1 + 0.2;
    printf("0.1 + 0.2 = %.17g; equal to 0.3: %s\n", decimal_sum,
           decimal_sum == 0.3 ? "yes" : "no");
    /* 显示的舍入结果和比较结果可能随实现而异。 */
    /* 枚举类型所兼容的整数类型由实现定义。 */

    section("arrays, structures, unions, pointers, and functions");
/*
 * 数组/结构说明：values 有 3 个 int 元素；假设/待确认：Point 的 x、y 坐标单位和参考系未提供，这里只演示字段访问；union 当前活动成员是 integer。
 */
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
/*
 * sizeof 边界：probe++ 的类型不是 VLA 类型，所以 sizeof 不执行自增表达式；expression_size 的单位为字节。
 */
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
/*
 * 除法边界：denominators 包含 0 和 4；循环先检查零，再执行除法/取余，避免除数为零。
 */
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
/*
 * 运算说明：赋值表达式从右向左结合，因此先令 c=10，再将结果赋给 b 和 a；三个变量最后都为 10。
 */
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
/*
 * 转换范围：-3.14 转成 int 会向 0 截断为 -3；本例值在 int 可表示范围内，超范围浮点转整数不应执行。
 */
    double decimal = -3.14;
    int whole = (int)decimal;
    printf("float-to-int truncates toward zero: %.2f -> %d\n", decimal, whole);
    /* 浮点值超出目标整数类型范围时，不要执行这种转换。 */
    printf("static-storage file object starts at %d\n", file_scope_count);
    return 0;
}
