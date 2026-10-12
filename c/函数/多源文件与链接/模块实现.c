/*
 * 文件用途：实现公开接口，并展示文件作用域 static、静态局部对象与外部定义。
 * 输入：函数调用传入的受控整数；本文件不直接读取标准输入。
 * 输出：无直接输出；由调用方观察返回值和共享计数。
 * 独立构建：gcc -std=c11 -Wall -Wextra -Wpedantic 模块实现.c 调用方.c -o 多源文件示例.exe
 */
#include "function_api.h"

/* 文件作用域 static 对象只在本翻译单元可见，并具有静态存储期。 */
static const int adjustment = 5;

/* 该定义为头文件中的 extern 声明提供唯一的外部定义。 */
unsigned int shared_call_count = 0U;

/*
 * 用途：把模块私有调整量加到小范围整数上。
 * 参数：subtotal 的有效范围为 [-2000, 2000]。
 * 返回值：subtotal + adjustment。
 * 异常/错误：C 不使用异常；调用者遵守范围约定时结果可由 int 表示。
 * 副作用：无。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 示例：公开函数 add_with_adjustment(6, 5) 内部以 subtotal=11 调用本函数，得到 16。
 */
static int apply_adjustment(int subtotal)
{
    return subtotal + adjustment;
}

/*
 * 用途：提供具有外部链接的加法接口，并调用当前翻译单元的私有辅助函数。
 * 参数：left、right 的有效范围均为 [-1000, 1000]。
 * 返回值：left + right + adjustment。
 * 异常/错误：C 不使用异常；不检查参数范围，调用者须使用约定范围。
 * 副作用：无；不修改共享计数。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 示例：调用方调用 add_with_adjustment(6, 5)，从其他翻译单元取得 16。
 */
int add_with_adjustment(int left, int right)
{
    int subtotal = left + right;
    return apply_adjustment(subtotal);
}

/*
 * 用途：保存并返回跨调用递增的序号。
 * 参数：无。
 * 返回值：递增后的 unsigned int 序号；无符号上限后按模数回绕。
 * 异常/错误：C 不使用异常；没有错误返回分支。
 * 副作用：静态局部 sequence 与外部 shared_call_count 各增加 1。
 * 复杂度：时间 O(1)，辅助空间 O(1)；只使用两个整数对象。
 * 示例：调用方连续调用两次，前两次分别得到 1 和 2，shared_call_count 变为 2。
 */
unsigned int next_sequence(void)
{
    /* 静态存储期对象未显式初始化时先置零，并在函数调用之间保留值。 */
    static unsigned int sequence;

    ++sequence;
    ++shared_call_count;
    return sequence;
}
