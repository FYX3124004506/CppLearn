/* =========================================================
 * 文件名：08_input_and_output.c
 * 描述：scanf 输入与 printf 输出综合练习
 * 知识点：字符串、整数、double 的读取与格式化输出
 * 说明：姓名示例使用 %31s，避免写入超过 name 数组容量。
 * ========================================================= */

#include <stdio.h>

int main(void) {
    int age;
    double height;
    char name[32];

    printf("请输入姓名（不含空格）：");
    if (scanf("%31s", name) != 1) {
        fprintf(stderr, "姓名读取失败。\n");
        return 1;
    }

    printf("请输入年龄：");
    if (scanf("%d", &age) != 1) {
        fprintf(stderr, "年龄读取失败。\n");
        return 1;
    }

    printf("请输入身高（米）：");
    if (scanf("%lf", &height) != 1) {
        fprintf(stderr, "身高读取失败。\n");
        return 1;
    }

    printf("\n========== 信息确认 ==========\n");
    printf("姓名：%s\n", name);
    printf("年龄：%d 岁\n", age);
    printf("身高：%.2f 米\n", height);
    printf("明年年龄：%d 岁\n", age + 1);

    return 0;
}
