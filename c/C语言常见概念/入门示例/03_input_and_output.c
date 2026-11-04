/*
 * 文件用途：展示 scanf 的地址参数、返回值检查和 printf 格式化输出。
 * 输入：一行中提供一个可由 int 表示的十进制整数；另用非数字文本观察失败分支。
 * 输出：输入成功时回显整数；转换失败时输出错误信息并返回非零状态。
 * 标准：C99 或更新版本。
 */
#include <stdio.h> /* 引入 scanf、printf 和 puts 的声明，供输入、提示及结果输出使用。 */

/*
 * 用途：读取一个整数；只有成功赋值后才使用 value。
 * 参数：无；示例从标准输入读取数据。
 * 返回值：int；有效整数返回 0，无法完成转换返回 1。
 * 错误处理：检查 scanf 的赋值数量；本例假定输入整数在 int 可表示范围内，不处理任意长文本或范围溢出。
 * 副作用：读取标准输入，并向标准输出写提示或结果。
 */
int main(void) {
    int value = 0; /* 创建并初始化 int 对象，作为 scanf 写入结果的位置。 */
    printf("Input one integer:\n"); /* 显示提示并换行，让输入提示与后续输出分行。 */
    int converted = scanf("%d", &value); /* %d 要求 int 指针；&value 取得对象地址；返回值是成功赋值的项目数。 */
    if (converted != 1) { /* 只有恰好读入一个整数时 converted 才等于 1；0 或 EOF 表示转换失败。 */
        puts("invalid input"); /* 失败时显示说明；puts 自动在文本后追加换行。 */
        return 1; /* 结束程序并用非零状态报告输入错误，避免继续使用未成功读入的值。 */
    }
    printf("value = %d\n", value); /* 转换成功后以 %d 输出 int 对象的当前值。 */
    return 0; /* 输入成功，结束 main 并报告正常状态。 */
}

/* 示例输入 1：42
 * 预期输出 1：
 * Input one integer:
 * value = 42
 * 示例输入 2：abc
 * 预期输出 2：
 * Input one integer:
 * invalid input
 * 关键结果：scanf 的 %d 在这里需要 int 对象地址；必须先检查返回的赋值数量，再使用输入值。
 */