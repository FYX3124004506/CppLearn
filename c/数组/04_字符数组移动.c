/*
用途：用字符数组演示字符从两端逐步向中间显示。
输入：无；使用固定字符串。
输出：每轮输出一个数组状态，最后完整显示 ARRAYS。
编译：gcc -std=c11 -Wall -Wextra -Wpedantic 04_字符数组移动.c -o array_motion
运行：./array_motion
*/
#include <stdio.h>
#include <string.h>

/*
用途：从固定字符串两端向中间填充字符，并逐轮打印可见状态。
参数：无（main 的参数列表为 void）。
返回值：所有轮次完成后返回 0。
异常/错误：C 不使用异常机制；本例没有运行时输入或失败分支，固定的非空字符串使 length-1 落在有效下标内。
副作用：每轮向 stdout 写一行字符数组状态。
复杂度：令 n 为 message 中非结束符字符数；时间 O(n)，两个局部字符数组合计占 O(n) 空间，循环辅助空间 O(1)。
示例：本函数先调用 strlen(message) 得到长度，再在循环中调用 puts(visible) 打印每轮状态。
*/
int main(void)
{
    /* length 的单位是字符数，不含结尾 '\0'；此固定字符串的 length 为 6。 */
    char message[] = "ARRAYS";
    size_t length = strlen(message);
    /* visible 容量按字节计为 sizeof message（本例 7），足以容纳 6 个字符和结束符。 */
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
    /* left/right 是字符下标；从 [0, length-1] 向内收缩，left<=right 时两端位置仍有效。 */
    for (size_t left = 0, right = length - 1;
         left <= right;
         ++left, --right) {
        visible[left] = message[left];
        visible[right] = message[right];
        puts(visible);
    }

    return 0;
}
