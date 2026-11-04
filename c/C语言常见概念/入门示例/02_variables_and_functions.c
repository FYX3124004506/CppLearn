/* 固定数据演示变量初始化、按值传参和函数返回值。 */
#include <stdio.h>

/* static 限制辅助函数只在当前源文件可见；参数接收调用时传入的 int 值。 */
static int add(int left, int right) {
    return left + right;
}

int main(void) {
    int first = 2;
    int second = 3;

    /* 调用时复制实参值给形参；add 返回的和用于初始化 total。 */
    int total = add(first, second);

    /* 受控编译反例：return 是关键字，不能用作变量名；取消下一行注释会编译失败。 */
    // int return = 10;

    printf("%d + %d = %d\n", first, second, total);
    return 0;
}

/* 示例输入：无；两个加数固定为 2 和 3。
 * 预期输出：
 * 2 + 3 = 5
 * 编译说明：活动代码可编译；若取消 int return = 10; 的注释，编译器会拒绝关键字作为标识符。
 */