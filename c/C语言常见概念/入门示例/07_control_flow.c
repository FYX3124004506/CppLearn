/* 固定数据综合演示分支、循环、continue、break 和 goto。 */
#include <stdio.h>

int main(void) {
    int score = 88;

    /* C 中零是假、非零为真；本例成绩满足 >= 60。 */
    if (score >= 60) {
        printf("passed = yes\n");
    } else {
        printf("passed = no\n");
    }

    /* switch 比较整数商；case 10 和 case 9 故意共用 A 级输出。 */
    switch (score / 10) {
        case 10:
        case 9:
            printf("grade band = A\n");
            break; // 结束最近的 switch，防止执行下一档。
        case 8:
            printf("grade band = B\n");
            break;
        default:
            printf("grade band = C\n");
            break;
    }

    int sum = 0;

    /* for 每轮先判断条件、执行循环体，再执行 ++number。 */
    for (int number = 1; number <= 5; ++number) {
        if (number == 3) {
            continue; // 跳过本轮累加；for 仍会递增 number 后检查下一轮。
        }
        sum += number;
    }
    printf("sum skipping 3 = %d\n", sum);

    /* while 先判断条件；每轮递减让条件最终变假。 */
    int countdown = 2;
    while (countdown > 0) {
        printf("while countdown = %d\n", countdown);
        --countdown;
    }

    /* do...while 先执行循环体；结尾分号属于语法的一部分。 */
    int attempts = 0;
    do {
        ++attempts;
        printf("do-while attempts = %d\n", attempts);
    } while (attempts < 1);

    int step = 1;

    /* break 会结束最近一层 while；step 保留触发退出的值 3。 */
    while (step <= 5) {
        if (step == 3) {
            printf("break loop at %d\n", step);
            break; // 结束最近的 while 循环，保留触发退出的 step 值。
        }
        printf("loop step = %d\n", step);
        ++step;
    }
    printf("after break step = %d\n", step);

    /* 固定数据使 sum 等于 12；goto 只作语法展示，日常代码优先用结构化流程。 */
    if (sum == 12) {
        goto finish;
    }
    printf("sum changed; goto branch skipped\n");

finish: // goto 的目标标签只标记位置，不执行计算。
    printf("goto reached finish\n");
    return 0;
}

/* 示例输入：无；score 固定为 88，循环范围和起始值固定。
 * 预期输出：
 * passed = yes
 * grade band = B
 * sum skipping 3 = 12
 * while countdown = 2
 * while countdown = 1
 * do-while attempts = 1
 * loop step = 1
 * loop step = 2
 * break loop at 3
 * after break step = 3
 * goto reached finish
 * 关键结果：if/switch 选择路径；continue 略过本轮，break 结束循环，do...while 至少执行一次。
 */
