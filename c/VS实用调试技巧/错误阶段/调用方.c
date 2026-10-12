/*
 * 文件用途：以两个翻译单元演示函数声明、编译和链接的配合。
 * 输入：无外部输入；固定调用参数为 12 和 30。
 * 输出：两数之和 42。
 * 独立编译：gcc -std=c11 -Wall -Wextra -Wpedantic 调用方.c 函数实现.c -o link_stages.exe
 * 运行：启动 link_stages.exe；两个源文件必须一起参与编译和链接。
 * 观察：原型让编译器检查调用形式；提供函数定义的翻译单元让链接器解析外部符号。
 */
#include <stdio.h>

int add_two_integers(int left, int right);

/*
 * 用途：程序入口，调用另一翻译单元提供的整数加法函数。
 * 参数：无。
 * 返回值：运算和输出成功返回 0。
 * 异常/错误：C 不使用异常；固定输入的和可由 int 表示，printf 错误未单独处理。
 * 副作用：调用 add_two_integers，并向标准输出写一行结果。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 用法：与函数实现.c 一起编译链接后直接运行，预期输出“12 + 30 = 42”。
 */
int main(void)
{
    const int left = 12;
    const int right = 30;
    const int sum = add_two_integers(left, right);

    printf("%d + %d = %d\n", left, right, sum);
    return 0;
}
