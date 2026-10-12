/*
 * C11 examples for common types, object sizes, numeric limits, and operators.
 * Input: none. Compile as C11 and run.
 * Ordinary identifiers are case-sensitive, start with a letter or underscore, and continue
 * with letters, digits, or underscores; keywords are reserved.
 * Scope and storage duration are language rules; memory placement is implementation-specific.
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

/* Integer and floating types are arithmetic; pointers are scalar types. */
/* Arrays and structures are aggregate types; unions are a separate category. */
/* _Bool and enumeration types are integer types. */
struct Point { int x; int y; };
union Number { int integer; float real; };
enum Status { STATUS_IDLE, STATUS_READY = 3, STATUS_DONE };

int file_scope_count; /* Tentative definition with static storage duration; zero initialized. */

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
    extern int file_scope_count; /* Declaration of the existing file-scope object. */
    section("character and integer types");
    char letter = 'A'; /* The execution character set determines its numeric value. */
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
    printf("FLT_DIG=%d, DBL_DIG=%d, LDBL_DIG=%d, FLT_MAX=%g\n",
           FLT_DIG, DBL_DIG, LDBL_DIG, (double)FLT_MAX);
    /* An enum's compatible integer type is implementation-defined. */

    section("arrays, structures, unions, pointers, and functions");
    int values[] = {2, 3, 5};
    struct Point point = {3, 4};
    union Number number = {.integer = 42};
    int (*operation)(int, int) = add;
    int *first = values;
    void *object_pointer = &point;

    printf("array=%d, struct=(%d,%d), union=%d, add(2,3)=%d\n",
           *first, point.x, point.y, number.integer, operation(2, 3));
    printf("object pointer=%p\n", (void *)object_pointer);
    /* A void pointer can hold an object pointer, but cannot be dereferenced directly. */

    section("sizeof and object sizes");
    int probe = 4;
    size_t expression_size = sizeof(probe++);
    /* Standard integer sizes are nondecreasing by rank; exact sizes vary by implementation. */
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
    double floating_quotient = 6 / 4.0;
    int remainder = -13 % 5;
    int negative = -1;
    unsigned int mixed = negative + 1U;
    unsigned int wrapped = UINT_MAX;

    wrapped += 1U;
    printf("9-4=%d, 3*4=%d; 6/4=%d, 6/4.0=%.1f, -13%%5=%d\n",
           difference, product, integer_quotient, floating_quotient, remainder);
    printf("mixed signed/unsigned=%u; UINT_MAX+1=%u\n", mixed, wrapped);
#if UCHAR_MAX < INT_MAX
    int promoted_sum = (unsigned char)UCHAR_MAX + 1;
    printf("unsigned char promotes to int: UCHAR_MAX+1=%d\n", promoted_sum);
#else
    unsigned int promoted_sum = (unsigned char)UCHAR_MAX + 1U;
    printf("unsigned char promotes to unsigned int: %u\n", promoted_sum);
#endif
    int denominator = 4;
    /* Check a denominator before division or remainder. */
    if (denominator != 0) {
        printf("guarded division/remainder: 20/%d=%d, 20%%%d=%d\n",
               denominator, 20 / denominator, denominator, 20 % denominator);
    }
    /* Signed overflow and INT_MIN / -1 are undefined; neither is executed. */

    section("assignment and increment");
    int a, b, c;
    a = b = c = 10;
    int count = 5;
    count += 3;
    count *= 2;
    /* Other compound forms: -=, /=, %=, <<=, >>=, &=, ^=, and |=. */
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
    /* Two unsequenced updates to the same scalar in one call would be undefined. */

    section("explicit conversion");
    double decimal = 3.14;
    int whole = (int)decimal;
    printf("float-to-int truncates toward zero: %.2f -> %d\n", decimal, whole);
    /* Do not convert a floating value outside the target integer range. */
    printf("static-storage file object starts at %d\n", file_scope_count);
    return 0;
}
