/* =========================================================
 * 文件名：01_hello_world.c
 * 描述：第一个 C 程序
 * 知识点：main 函数、标准输出、返回值和换行转义字符
 * 编译：gcc -std=c11 -Wall -Wextra -Wpedantic 01_hello_world.c -o 01_hello_world.exe
 * ========================================================= */

#include <stdio.h>

int main(void) {
    // main 是 C 程序的执行入口；标准形式明确返回 int 且不接收参数。
    printf("Hello, World!\n");  // \n 表示换行。

    // 返回 0 表示程序正常结束。
    return 0;
}
