/*
用途：用字符数组演示字符从两端逐步向中间显示。
输入：无；使用固定字符串。
输出：每轮输出一个数组状态，最后完整显示 ARRAYS。
编译：gcc -std=c11 -Wall -Wextra -Wpedantic 04_字符数组移动.c -o array_motion
运行：./array_motion
*/
#include <stdio.h>
#include <string.h>

int main(void)
{
    char message[] = "ARRAYS";
    size_t length = strlen(message);
    char visible[sizeof message];

    /*
    message 的数组长度包含末尾的 '\0'，strlen 统计其前的字符数。
    visible 每轮从两端写入字符，并始终保留字符串结束符。
    */
    for (size_t i = 0; i < length; ++i) {
        visible[i] = ' ';
    }
    visible[length] = '\0';

    /*
    为保证示例可移植且输出可复核，这里逐行打印，不调用清屏命令。
    POSIX sleep() 参数以秒计，Windows Sleep() 参数以毫秒计；
    system("cls") 依赖 Windows 命令解释器，并非通用的 C 清屏方式。
    */
    for (size_t left = 0, right = length - 1;
         left <= right;
         ++left, --right) {
        visible[left] = message[left];
        visible[right] = message[right];
        puts(visible);
    }

    return 0;
}
