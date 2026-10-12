/*
 * 演示变量声明、赋值、作用域和存储期。
 * 输入：无。使用 C11 编译后运行。
 * 标识符区分大小写，不能以数字开头，也不能使用关键字。
 * 作用域和存储期由语言规则定义；对象的具体内存位置由实现决定。
 */
#include <stdio.h>

struct Record { int id; double score; };

int total_visits; /* 文件作用域对象具有静态存储期；未显式初始化时会被零初始化。 */
int default_values[3];
struct Record default_record;

static void record_visit(void)
{
    static int visit_count; /* 函数调用结束后仍保留值；未显式初始化时从零开始。 */

    ++visit_count;
    printf("visit=%d\n", visit_count);
}

int main(void)
{
    extern int total_visits; /* 声明上方已经定义的文件作用域对象。 */

    puts("-- initialization and assignment --");
    int age = 18;
    double price = 12.5;
    char grade = 'A';
    char grade_2 = 'B';
    int score; /* 自动存储期局部对象先写入确定值，再读取。 */
    score = 95.75; /* 赋值时转换为 int，小数部分向零截断。 */
    printf("age=%d, price=%.2f, grade=%c/%c\n", age, price, grade, grade_2);
    printf("score after assignment=%d\n", score);

    puts("-- identifier case and block scope --");
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
