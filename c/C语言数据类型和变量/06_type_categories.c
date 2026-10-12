/*
 * 用途：用实际对象展示 C 类型分类，以及对象指针与函数指针的区别。
 * 输入：无；程序使用固定变量和枚举值。输出：分类示例、值和当前实现的字符属性。
 * 编译运行：gcc -std=c11 -Wall -Wextra -Wpedantic 06_type_categories.c -o 06_type_categories.exe
 */
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

struct Point {
    int x;
    int y;
};

union Number {
    int integer;
    double real;
};

enum Status {
    STATUS_IDLE = 0,
    STATUS_READY = 2,
    STATUS_DONE
};

static int add(int left, int right)
{
    return left + right;
}

/* void 是无返回值类型；函数执行动作，但不产生可供保存的返回值。 */
static void print_status(enum Status status)
{
    printf("状态函数收到枚举值：%d\n", (int)status);
}

int main(void)
{
    puts("-- 算术类型与枚举 --");
    char letter = 'A';
    signed char signed_value = -10;
    unsigned char byte_value = 250U;
    int count = 7;
    double ratio = 1.5;
    _Bool normalized = 42;
    bool ready = true;
    enum Status status = STATUS_DONE;

    /* 整数类型与浮点类型都属于算术类型；_Bool 和枚举也属于整数类型。 */
    /* 字符常量的执行字符集数值与普通 char 的符号性都由实现决定。 */
    printf("字符=%c，执行字符集数值=%d；普通 char 在本实现中为%s\n",
           letter, (int)letter, CHAR_MIN < 0 ? "有符号" : "无符号");
    printf("signed char=%d，unsigned char=%u，int=%d，double=%.1f\n",
           (int)signed_value, (unsigned int)byte_value, count, ratio);
    printf("_Bool 把非零值规范为%d；bool 别名的值为%s\n",
           (int)normalized, ready ? "true" : "false");
    printf("枚举隐式后续值 STATUS_DONE=%d\n", (int)status);
    print_status(status);

    puts("-- 聚合类型、联合体和派生类型 --");
    int values[3] = {10, 20, 30};
    struct Point point = {3, 4};
    union Number number = {.integer = 42};

    /* 数组和结构体是聚合类型；联合体单独分类，只读取最近写入的成员。 */
    printf("数组首元素=%d；结构体坐标=(%d,%d)；联合体整数成员=%d\n",
           values[0], point.x, point.y, number.integer);

    /* 数组、指针和函数类型属于派生类型；数组和函数指针在声明中可见。 */
    int *first_value = values;
    int (*operation)(int, int) = add;
    printf("指针访问数组元素=%d；函数指针调用 add(2,3)=%d\n",
           *first_value, operation(2, 3));

    puts("-- void 指针只通用于对象指针 --");
    void *generic_object_pointer = &count;
    int *restored_pointer = generic_object_pointer; /* C 允许对象指针与 void* 往返转换。 */
    printf("对象指针往返后仍指向同一对象=%s，值=%d\n",
           restored_pointer == &count ? "是" : "否", *restored_pointer);
    printf("对象指针输出=%p\n", (void *)generic_object_pointer);
    /* void* 不能直接解引用或做指针算术；函数指针也不当作对象指针使用。 */

    return 0;
}
