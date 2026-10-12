/*
 * 演示变量声明、赋值、作用域和存储期。
 * 输入：无。使用 C11 编译后运行。
 * 标识符区分大小写，不能以数字开头，也不能使用关键字。
 * 作用域和存储期由语言规则定义；对象的具体内存位置由实现决定。
 */
/* 输出：变量赋值、遮蔽、静态默认值、地址观察结果，以及两次访问计数。 */
#include <stdio.h>

struct Record { int id; double score; };

int total_visits; /* 文件作用域对象具有静态存储期；未显式初始化时会被零初始化。 */
int default_values[3];
struct Record default_record;

/*
 * 用途：累加并显示函数局部静态访问计数。
 * 参数：无。
 * 返回值：无（void）。
 * 异常/错误：C 无异常机制；此函数不产生错误返回，标准输出错误未检查。
 * 副作用：visit_count 在调用间保留并递增；向 stdout 写一行。
 * 复杂度：时间 O(1)，辅助空间 O(1)；每次只操作一个静态计数器。
 * 示例：main 先后调用 record_visit() 两次，观察计数从 1 到 2。
 */
static void record_visit(void)
{
    /* visit_count 是累计调用次数；本例实际范围为 0..2，类型上限为 INT_MAX。 */
    static int visit_count; /* 函数调用结束后仍保留值；未显式初始化时从零开始。 */

    ++visit_count;
    printf("visit=%d\n", visit_count);
}

/*
 * 用途：演示初始化、赋值转换、名称遮蔽和静态存储期。
 * 参数：无。
 * 返回值：0 表示示例正常结束。
 * 异常/错误：C 无异常机制；本例无输入或可恢复错误，标准输出错误未检查。
 * 副作用：向 stdout 输出示例；调用 record_visit 两次并改变其静态计数。
 * 复杂度：时间 O(1)，辅助空间 O(1)；所有示例对象和输出次数固定。
 * 示例：运行本文件进入 main(void)，按初始化、作用域、存储期顺序观察输出。
 */
int main(void)
{
    extern int total_visits; /* 声明上方已经定义的文件作用域对象。 */

    puts("-- initialization and assignment --");
    /* age 单位为年；假设/待确认：price 的币种未提供；这里仅演示 double 类型，不代表业务金额或范围。 */
    int age = 18;
    double price = 12.5;
    char grade = 'A';
    char grade_2 = 'B';
    /* 假设/待确认：score 没有指定评分制；本例只展示 double 转为 int 的结果，不代表具体评分规则。 */
    int score; /* 自动存储期局部对象先写入确定值，再读取。 */
    score = 95.75; /* 赋值时转换为 int，小数部分向零截断。 */
    printf("age=%d, price=%.2f, grade=%c/%c\n", age, price, grade, grade_2);
    printf("score after assignment=%d\n", score);

    puts("-- identifier case and block scope --");
    /* count 与 Count 是两个独立对象；两个例值范围分别为 5 和 8。 */
    int count = 5;
    int Count = 8;
    int item2 = 9;
    printf("case-sensitive identifiers: count=%d, Count=%d\n", count, Count);
    printf("valid identifier with a digit after its first letter: item2=%d\n", item2);
    {
        int count = 12; /* 这个块作用域名称遮蔽外层同名的 count。 */
        printf("inner count=%d\n", count);
    }
    printf("outer count=%d, Count=%d\n", count, Count);

    puts("-- static storage duration --");
    printf("file-scope total_visits starts at %d\n", total_visits);
    printf("static aggregate defaults: array=%d,%d,%d; record=(%d,%.1f)\n",
           default_values[0], default_values[1], default_values[2],
           default_record.id, default_record.score);
    /* 地址仅用于观察当前实现；C 标准不承诺对象所在的具体内存区域。 */
    printf("addresses observed on this run (implementation-specific): file=%p, automatic=%p\n",
           (void *)&total_visits, (void *)&age);
    record_visit();
    record_visit();
    return 0;
}
