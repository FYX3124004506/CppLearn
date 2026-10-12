/* =========================================================
 * 文件名：09_statements_and_control.c
 * 描述：语句类型与基本控制结构示例
 *       演示顺序、选择、循环、break、continue、goto 和 return
 * 编译：gcc -std=c11 -Wall -Wextra -Wpedantic 09_statements_and_control.c -o 09_statements_and_control.exe
 * ========================================================= */

/*
 * 文件用途：演示顺序、选择、循环、break、continue、goto 和 return。
 * 输入：无；局部变量和循环边界均由程序设定。
 * 输出：标准输出按控制流显示各示例结果。
 */
#include <stdio.h>

/*
 * 用途：返回两个 int 参数的和，供顺序结构示例调用。
 * 参数：left、right 是参与相加的 int 值；相加结果必须可由 int 表示。
 * 返回值：int；返回 left + right。
 * 异常/错误：C 不使用异常机制；函数不检查有符号溢出，调用者须保证结果在 int 范围内。
 * 副作用：无；只计算并返回结果。
 * 复杂度：时间 O(1)，辅助空间 O(1)；只进行一次加法。
 * 示例：main 令 left=2、right=3，再调用 add(left, right) 得到 sum=5。
 */
static int add(int left, int right) {
    return left + right;
}

/*
 * 用途：按顺序运行空语句、选择、循环和跳转示例。
 * 参数：无；所有判断值和循环边界均在本函数内设置。
 * 返回值：int；返回 0 表示正常结束。
 * 异常/错误：C 不使用异常机制；无外部输入；add 的两个实际参数相加不溢出，输出错误未检查。
 * 副作用：向标准输出写入示例结果，并修改局部计数变量。
 * 复杂度：时间 O(1)，辅助空间 O(1)；循环次数固定为 3、5、2、1 等常数，没有按输入增长的数据结构。
 * 示例：运行本程序会调用 add(2, 3)；sum 为 5，因此实际执行 goto cleanup，跳过其后的条件提示语句。
 */
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
/*
 * 循环范围：i 从 0 到 2，共 3 次；i==1 时 continue 跳过本轮 printf，但 for 的增量表达式仍会执行。
 */
    for (int i = 0; i < 3; ++i) {
        if (i == 1) {
            continue;
        }
        printf(" %d", i);
    }
    printf("\n");

    /* break 会结束所在的最近一层循环。 */
    printf("for 与 break：");
/*
 * 循环范围：i 从 0 开始；当 i==3 时先 break，因此只打印 0、1、2。
 */
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
/*
 * 本例边界：left 和 right 固定为 2、3，所以 sum 恒为 5；当前运行一定跳到 cleanup，标签后的 printf 不会执行。
 */
    if (sum == 5) {
        goto cleanup;
    }
    printf("这行在 sum 不等于 5 时执行\n");

cleanup:
    printf("goto：已跳转到 cleanup 标签\n");
    return 0;
}
