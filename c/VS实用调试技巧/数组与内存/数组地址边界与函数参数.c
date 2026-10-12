/*
 * 文件用途：观察数组到指针转换、显式长度、有效下标和不同存储期对象的地址。
 * 输入：无外部输入；数组内容固定，动态数组由程序申请。
 * 输出：数组值、大小、地址和下标有效性；地址数值依运行环境而变化。
 * 独立编译：gcc -std=c11 -Wall -Wextra -Wpedantic 数组地址边界与函数参数.c -o array_memory.exe
 * 运行：启动程序；调试暂停后可在监视窗口观察 values、values[0]、length。
 * 注意：程序只打印有效对象地址，不比较不同对象的地址顺序，也不执行越界访问。
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int file_scope_value = 100;

/*
 * 用途：判断从 0 开始的数组下标是否落在有效范围内。
 * 参数：index 是 size_t 下标，范围为 0 到 SIZE_MAX；length 是元素个数。
 * 返回值：index < length 时返回 1，否则返回 0。
 * 异常/错误：C 不使用异常；所有 size_t 输入都可处理，空数组长度为 0 时无下标有效。
 * 副作用：无。
 * 复杂度：时间 O(1)，辅助空间 O(1)。
 * 用法：main 对 length=4 分别查询下标 3 和 4，输出有效与无效。
 */
static int is_valid_index(size_t index, size_t length)
{
    return index < length;
}

/*
 * 用途：通过数组形参遍历并打印元素，展示函数调用时必须显式传入长度。
 * 参数：values 指向至少 length 个可读 int 的连续区域；length 是元素数，本例为 4，输出转换要求它能由 unsigned long 表示。
 * 返回值：成功输出返回 1；length 大于 0 且 values 为空时返回 0。
 * 异常/错误：C 不使用异常；空指针与正长度组合会被拒绝，其他指针范围由调用者保证。
 * 副作用：向标准输出写数组地址、长度和每个元素。
 * 复杂度：N 为 length，时间 O(N)，辅助空间 O(1)。
 * 用法：main 将 values 数组和由 sizeof 计算的 length 一起传入。
 */
static int print_array(const int values[], size_t length)
{
    if (values == NULL && length > 0)
    {
        return 0;
    }

    printf("函数收到的首元素地址：%p，显式传入长度：%lu\n",
           (void *)values, (unsigned long)length);

    for (size_t i = 0; i < length; ++i)
    {
        printf("%d%s", values[i], i + 1 == length ? "\n" : " ");
    }

    if (length == 0)
    {
        putchar('\n');
    }

    return 1;
}

/*
 * 用途：展示数组、元素、文件作用域对象、局部对象和动态分配对象的安全观察方式。
 * 参数：无。
 * 返回值：正常完成返回 0；动态分配失败返回 1；未拒绝无效参数或有效数组打印失败返回 2。
 * 异常/错误：C 不使用异常；检查 malloc 结果和 print_array 的状态返回值。
 * 副作用：检查空指针参数并向标准输出打印数据和地址；申请并释放动态内存。
 * 复杂度：N=4 是主数组长度、M=10 是边界示例长度；时间与辅助空间均为 O(N+M)。
 * 用法：直接运行程序；values == &values[0] 应输出“是”，下标 3 有效、下标 4 无效。
 */
int main(void)
{
    int local_value = 200;
    int values[4] = {10, 20, 30, 40};
    int ten_values[10] = {0};
    int (*whole_array_pointer)[4] = &values;
    int *first_element_pointer = values;
    int *dynamic_values = malloc(4 * sizeof *dynamic_values);
    const unsigned char *object_bytes = (const unsigned char *)&values[0];

    if (dynamic_values == NULL)
    {
        fputs("动态内存分配失败\n", stderr);
        return 1;
    }

    for (size_t i = 0; i < 4; ++i)
    {
        dynamic_values[i] = values[i] * 10;
    }

    const size_t length = sizeof values / sizeof values[0];
    const size_t ten_length = sizeof ten_values / sizeof ten_values[0];

    /* 本示例固定尺寸很小，转换为 unsigned long 后用 %lu 输出。 */

    printf("数组表达式等于首元素地址：%s\n",
           values == &values[0] ? "是" : "否");
    printf("同一数组相邻元素的指针距离：%lu 个元素\n",
           (unsigned long)(&values[1] - &values[0]));
    printf("数组指针所指对象大小：%lu 字节；元素指针所指对象大小：%lu 字节\n",
           (unsigned long)sizeof *whole_array_pointer,
           (unsigned long)sizeof *first_element_pointer);
    printf("sizeof values=%lu 字节，sizeof values[0]=%lu 字节\n",
           (unsigned long)sizeof values, (unsigned long)sizeof values[0]);
    printf("int[4] 数组类型的对齐要求：%lu 字节\n",
           (unsigned long)_Alignof(int[4]));
    printf("首元素对象表示字节：");
    for (size_t byte_index = 0; byte_index < sizeof values[0]; ++byte_index)
    {
        printf(" %02X", (unsigned int)object_bytes[byte_index]);
    }
    putchar('\n');
    printf("sizeof(void*)=%lu 字节（当前目标平台）\n", (unsigned long)sizeof(void *));
    printf("文件作用域对象地址：%p\n", (void *)&file_scope_value);
    printf("局部对象地址：%p\n", (void *)&local_value);
    printf("数组首元素地址：%p\n", (void *)&values[0]);
    printf("动态分配对象地址：%p\n", (void *)dynamic_values);
    printf("下标 3 有效：%s\n",
           is_valid_index(3, length) ? "是" : "否");
    printf("下标 4 有效：%s\n",
           is_valid_index(4, length) ? "是" : "否");
    printf("长度为10时，下标9有效：%s；下标10有效：%s\n",
           is_valid_index(9, ten_length) ? "是" : "否",
           is_valid_index(10, ten_length) ? "是" : "否");
    if (is_valid_index(9, ten_length))
    {
        printf("安全读取下标9：%d\n", ten_values[9]);
    }

    if (print_array(NULL, 1))
    {
        fputs("错误：未拒绝空指针与正长度组合\n", stderr);
        free(dynamic_values);
        return 2;
    }
    puts("空指针与正长度组合：已拒绝");
    puts("通过数组形参访问原数组：");
    if (!print_array(values, length))
    {
        free(dynamic_values);
        return 2;
    }

    puts("通过指针访问动态数组：");
    if (!print_array(dynamic_values, length))
    {
        free(dynamic_values);
        return 2;
    }

    free(dynamic_values);
    return 0;
}
