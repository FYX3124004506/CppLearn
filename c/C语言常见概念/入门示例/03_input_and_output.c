/* 读取一个整数，先检查 scanf 的结果，再使用读入值。 */
#include <stdio.h>

int main(void) {
    int value = 0;
    printf("Input one integer:\n");

    /* %d 对应 int*；&value 传入对象地址。返回值表示成功赋值的项目数。 */
    int converted = scanf("%d", &value);

    /* 只有返回 1 时 value 才成功读入；失败就结束，避免使用无效输入。 */
    if (converted != 1) {
        puts("invalid input");
        return 1;
    }

    printf("value = %d\n", value);
    return 0;
}

/* 示例输入 1：42
 * 预期输出 1：
 * Input one integer:
 * value = 42
 * 示例输入 2：abc
 * 预期输出 2：
 * Input one integer:
 * invalid input
 * 关键结果：先检查 scanf 的赋值数量，再使用 value。
 */