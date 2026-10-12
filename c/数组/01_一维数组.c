/*
用途：演示一维数组的声明、初始化、访问、长度计算、地址和受控输入。
输入：依次输入 3 个整数，空格或回车均可分隔。
输出：数组元素、元素数、字节数、地址、复制结果和输入结果。
编译：gcc -std=c11 -Wall -Wextra -Wpedantic 01_一维数组.c -o array_1d
运行：./array_1d
*/
#include <stddef.h>
#include <stdio.h>

/*
用途：把指定数量的整数依次打印在一行，便于 main 展示不同数组。
参数：label 是输出标签；values 指向至少 count 个 int 元素；count 的单位是元素个数，允许为 0。
返回值：无（void）。
异常/错误：C 不使用异常机制；本函数不检查指针和容量，调用者须保证 count 大于 0 时 values 有效，label 指向有效字符串。
副作用：向标准输出写入标签、元素和换行符。
复杂度：令 n=count；时间 O(n)，辅助空间 O(1)，只使用循环下标。
示例：main 中 print_values("完整初始化", complete, complete_count) 按 complete_count 个元素打印 complete。
*/
/*
数组形参会调整为指针，因此调用者要把元素个数一并传入。
函数依靠 count 控制访问范围，不在形参内部用 sizeof 推算长度。
*/
static void print_values(const char *label, const int values[], size_t count)
{
    printf("%s:", label);
    for (size_t i = 0; i < count; ++i) {
        printf(" %d", values[i]);
    }
    putchar('\n');
}

/* 静态存储期对象即使没有显式初始化，元素也会初始化为零。 */
static int static_values[3];

/*
用途：展示数组初始化、元素个数和地址等信息，然后读取并打印 3 个整数。
参数：无（main 的参数列表为 void）。
返回值：全部输入成功时返回 0；任一次整数读取失败时返回 1。
异常/错误：C 不使用异常机制；逐个检查 scanf 的转换数，输入不足或格式错误时向 stderr 报错并提前返回 1。
副作用：向 stdout 打印演示结果和提示；输入错误时向 stderr 打印诊断；从 stdin 读取 3 个整数。
复杂度：本程序的示例数组和输入长度均固定，时间与辅助空间均为 O(1)；各遍历按对应数组元素数线性执行。
示例：运行程序并在提示后输入“1 2 3”，main 会检查三次 scanf 并打印这三个值。
*/
int main(void)
{
    /* 数组类型由元素类型和元素个数组成，长度为 4。 */
    int complete[4] = {12, 18, 24, 30};
    int partial[4] = {7, 9};       /* 未写出的元素自动补为 0 */
    int inferred[] = {3, 6, 9};    /* 长度由初始化项推导为 3 */
    char letters[5] = {'a', 'b'}; /* 后续字符为 '\0' */

    /* complete_count 的单位是元素个数（本例为 4）；sizeof 的结果单位是字节。 */
    size_t complete_count = sizeof complete / sizeof complete[0];

    print_values("完整初始化", complete, complete_count);
    print_values("部分初始化", partial, sizeof partial / sizeof partial[0]);
    print_values("推导长度", inferred, sizeof inferred / sizeof inferred[0]);
    print_values("静态数组", static_values,
                 sizeof static_values / sizeof static_values[0]);

    printf("letters[2] 的字符码：%u\n",
           (unsigned int)(unsigned char)letters[2]);
    printf("元素个数：%lu\n", (unsigned long)complete_count);
    printf("整个数组字节数：%lu\n", (unsigned long)sizeof complete);
    printf("单个元素字节数：%lu\n", (unsigned long)sizeof complete[0]);

    /*
    当前 MinGW GCC 8.1 的格式检查会对 %zu 报警；实测本机运行库仍
    输出预期值。这里保留一行 %zu 展示标准写法，其他小尺寸显式转为
    unsigned long 后用 %lu，避免同一旧格式检查问题反复出现。
    */
    printf("标准 size_t 格式：%zu\n", complete_count);
    printf("有效下标范围：0 到 %lu\n",
           (unsigned long)(complete_count - 1));
    printf("首个元素：%d，最后一个有效元素：%d\n",
           complete[0], complete[complete_count - 1]);

    for (size_t i = 0; i < complete_count; ++i) {
        printf("complete[%lu] 地址：%p\n",
               (unsigned long)i, (void *)&complete[i]);
    }
    printf("相邻元素指针差：%ld 个元素\n",
           (long)(&complete[1] - &complete[0]));

    /*
    数组不能整体赋值；逐元素复制能清楚展示每个目标元素如何取得值。
    */
    int copied[4];
    for (size_t i = 0; i < complete_count; ++i) {
        copied[i] = complete[i];
    }
    print_values("逐元素复制", copied, complete_count);

    /*
    局部数组在成功读入前没有确定值；只有全部读取成功后才打印。
    scanf 中的空白可匹配空格或换行，&values[i] 指向待写入的元素。
    */
    int input_values[3];
    puts("请输入 3 个整数（空格或回车分隔）：");
    /* i 是 input_values 的下标，范围 [0, 3)；每次只在确认 scanf 成功后才继续，避免打印未写入的元素。 */
    for (size_t i = 0; i < 3; ++i) {
        if (scanf("%d", &input_values[i]) != 1) {
            fputs("输入失败：请提供 3 个整数。\n", stderr);
            return 1;
        }
    }
    print_values("读入结果", input_values, 3);

    return 0;
}
