/* 用固定数据先练习一个简单条件和循环。 */
#include <stdio.h>

int main(void) {
    int score = 85;

    /* >= 表示大于或等于；条件成立时进入 if，否则执行 else。 */
    if (score >= 60) {
        puts("pass");
    } else {
        puts("try again");
    }

    int sum = 0;

    /* for 的三部分依次设起点、继续条件和每轮更新；这里累加 1、2、3。 */
    for (int value = 1; value <= 3; ++value) {
        sum += value; // 把当前值加到 sum 并存回，等价于 sum = sum + value。
    }

    printf("sum = %d\n", sum);
    return 0;
}

/* 示例输入：无；score 固定为 85。
 * 预期输出：
 * pass
 * sum = 6
 */