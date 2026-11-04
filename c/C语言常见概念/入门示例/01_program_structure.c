/* 固定数据示范 C 程序入口、整数初始化和标准输出。 */
#include <stdio.h>

/* void 明确 main 不接收参数；int 返回 0 让宿主环境识别正常结束。 */
int main(void) {
    int item_count = 3;
    printf("item_count = %d\n", item_count);
    return 0;
}

/* 示例输入：无。
 * 预期输出：
 * item_count = 3
 * 关键结果：item_count 初始化为 3，printf 用 %d 输出 int 值。
 */