/*
 * Demonstrate variable declaration, assignment, block scope, and storage duration.
 */

#include <stdio.h>

int total_visits; /* 文件作用域对象具有静态存储期，默认初始化为零。 */

static void record_visit(void)
{
    static int visit_count; /* 静态局部变量会在多次调用之间保留值。 */

    ++visit_count;
    printf("visit=%d\n", visit_count);
}

int main(void)
{
    int age = 18;
    double price = 12.5;
    char grade = 'A';

    int score; /* 自动局部变量先赋值，再读取。 */
    score = 95;

    printf("age=%d, price=%.2f, grade=%c\n", age, price, grade);
    printf("score after assignment=%d\n", score);

    int count = 5;
    {
        int count = 12; /* 内层同名变量遮蔽外层变量。 */
        printf("inner count=%d\n", count);
    }
    printf("outer count after block=%d\n", count);

    printf("global total_visits=%d\n", total_visits);
    record_visit();
    record_visit();

    return 0;
}
