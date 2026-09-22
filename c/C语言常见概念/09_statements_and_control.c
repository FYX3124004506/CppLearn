/* =========================================================
 * 文件名：09_statements_and_control.c
 * 描述：语句类型与基本控制结构示例
 *       演示顺序、选择、循环、break、continue、goto 和 return
 * 编译：gcc -std=c11 -Wall -Wextra -Wpedantic 09_statements_and_control.c -o 09_statements_and_control.exe
 * ========================================================= */

#include <stdio.h>

static int add(int left, int right) {
    return left + right;
}

int main(void) {
    ; /* 空语句：不执行任何操作 */

    /* 声明、初始化和函数调用可以出现在同一条语句中。 */
    int left = 2;
    int right = 3;
    int sum = add(left, right);
    printf("顺序结构：%d + %d = %d\n", left, right, sum);

    /* 选择结构：if 根据条件选择执行路径。 */
    if (sum > 4) {
        printf("if：sum 大于 4\n");
    } else {
        printf("if：sum 不大于 4\n");
    }

    /* switch 中通常使用 break 防止继续执行下一个 case。 */
    switch (sum) {
        case 5:
            printf("switch：sum 等于 5\n");
            break;
        default:
            printf("switch：其他值\n");
            break;
    }

    /* for 循环中的 continue 会跳过本轮循环剩余代码。 */
    printf("for 与 continue：");
    for (int i = 0; i < 3; ++i) {
        if (i == 1) {
            continue;
        }
        printf(" %d", i);
    }
    printf("\n");

    /* break 会结束所在的最近一层循环。 */
    printf("for 与 break：");
    for (int i = 0; i < 5; ++i) {
        if (i == 3) {
            break;
        }
        printf(" %d", i);
    }
    printf("\n");

    /* while 可能一次也不执行。 */
    int count = 0;
    while (count < 2) {
        printf("while：count = %d\n", count);
        ++count;
    }

    /* do...while 至少执行一次循环体。 */
    int runs = 0;
    do {
        printf("do...while：至少执行一次\n");
        ++runs;
    } while (runs < 1);

    /* goto 只能跳转到同一函数内的标签，本例仅用于演示语法。 */
    if (sum == 5) {
        goto cleanup;
    }
    printf("这行在 sum 不等于 5 时执行\n");

cleanup:
    printf("goto：已跳转到 cleanup 标签\n");
    return 0;
}
