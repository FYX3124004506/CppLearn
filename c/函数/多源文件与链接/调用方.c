/*
 * 文件用途：通过共享声明调用另一个翻译单元中的函数和外部对象。
 * 输入：无；调用参数固定。
 * 输出：调整后的加法结果、两个连续序号和共享调用计数。
 * 独立构建：在本目录运行 gcc -std=c11 -Wall -Wextra -Wpedantic 模块实现.c 调用方.c -o 多源文件示例.exe
 */
#include <stdio.h>
#include "function_api.h"

/*
 * 用途：作为程序入口，调用公开函数并观察跨翻译单元共享状态。
 * 参数：无；void 表示不接收启动参数。
 * 返回值：int；所有输出成功返回 0，printf 失败返回 1。
 * 异常/错误：C 不使用异常；检查 printf 的负返回值。
 * 副作用：向标准输出写入结果；通过 next_sequence 更新共享对象。
 * 复杂度：时间 O(1)，辅助空间 O(1)；只进行固定数量的调用与输出。
 * 示例：编译并链接实现单元与本调用单元后运行，无需额外输入。
 */
int main(void)
{
    int adjusted_sum = add_with_adjustment(6, 5);
    unsigned int first_sequence = next_sequence();
    unsigned int second_sequence = next_sequence();

    if (printf("adjusted sum: %d\n", adjusted_sum) < 0) {
        return 1;
    }
    if (printf("sequence: %u, %u; shared calls: %u\n",
               first_sequence, second_sequence, shared_call_count) < 0) {
        return 1;
    }

    return 0;
}
