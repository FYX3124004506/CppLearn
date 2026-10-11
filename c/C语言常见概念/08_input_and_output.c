/* 练习受限输入、数值解析、范围检查和格式化输出。 */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 读取单个名字，并拒绝超出缓冲区的输入。 */
static int read_name(char *name, size_t capacity)
{
    size_t length = 0;
    int ch;

    do {
        ch = getchar();
    } while (ch != EOF && isspace((unsigned char)ch));

    if (ch == EOF) {
        return 0;
    }

    do {
        if (length + 1 >= capacity) {
            while (ch != EOF && !isspace((unsigned char)ch)) {
                ch = getchar();
            }
            return 0;
        }
        name[length++] = (char)ch;
        ch = getchar();
    } while (ch != EOF && !isspace((unsigned char)ch));

    name[length] = '\0';
    return 1;
}

/* 先读入受限字符串，再检查转换结果和 int 范围。 */
static int read_int_token(int *result)
{
    char text[64];
    char *end;
    long value;

    if (scanf("%63s", text) != 1 || strlen(text) == sizeof text - 1) {
        return 0;
    }

    errno = 0;
    value = strtol(text, &end, 10);
    if (errno == ERANGE || end == text || *end != '\0' ||
        value < INT_MIN || value > INT_MAX) {
        return 0;
    }

    *result = (int)value;
    return 1;
}

static int read_height(double *result)
{
    char text[64];
    char *end;
    double value;

    if (scanf("%63s", text) != 1 || strlen(text) == sizeof text - 1) {
        return 0;
    }

    errno = 0;
    value = strtod(text, &end);
    if (errno == ERANGE || end == text || *end != '\0' || !isfinite(value) ||
        value < 0.0) {
        return 0;
    }

    *result = value;
    return 1;
}

int main(void)
{
    int age;
    double height;
    char name[32];

    printf("请输入姓名（不含空格，最多 31 字节）：");
    if (!read_name(name, sizeof name)) {
        fprintf(stderr, "姓名无效或过长。\n");
        return 1;
    }

    printf("请输入年龄：");
    if (!read_int_token(&age) || age < 0) {
        fprintf(stderr, "年龄输入无效。\n");
        return 1;
    }

    printf("请输入身高（米）：");
    if (!read_height(&height)) {
        fprintf(stderr, "身高输入无效。\n");
        return 1;
    }

    printf("\n========== 信息确认 ==========\n");
    printf("姓名：%s\n", name);
    printf("年龄：%d 岁\n", age);
    printf("身高：%.2f 米\n", height);

    /* 边界值先检查，避免有符号整数溢出。 */
    if (age == INT_MAX) {
        printf("明年年龄：超出 int 范围\n");
    } else {
        printf("明年年龄：%d 岁\n", age + 1);
    }

    return 0;
}
