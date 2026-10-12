/*
 * Demonstrate variable declarations, assignment, scope, and storage duration.
 * Input: none. Compile as C11 and run.
 * Common identifiers are case-sensitive, do not start with a digit, and cannot be keywords.
 * Scope and storage duration are language rules; memory placement is implementation-specific.
 */
#include <stdio.h>

struct Record { int id; double score; };

int total_visits; /* File-scope definition with static storage duration; zero initialized. */
int default_values[3];
struct Record default_record;

static void record_visit(void)
{
    static int visit_count; /* Retains its value between calls and starts at zero. */

    ++visit_count;
    printf("visit=%d\n", visit_count);
}

int main(void)
{
    extern int total_visits; /* Declaration of the file-scope object defined above. */

    puts("-- initialization and assignment --");
    int age = 18;
    double price = 12.5;
    char grade = 'A';
    char grade_2 = 'B';
    int score; /* Assign a known value before reading an automatic local object. */
    score = 95.75; /* Assignment converts the value to int, truncating toward zero. */
    printf("age=%d, price=%.2f, grade=%c/%c\n", age, price, grade, grade_2);
    printf("score after assignment=%d\n", score);

    puts("-- identifier case and block scope --");
    int count = 5;
    int Count = 8;
    printf("case-sensitive identifiers: count=%d, Count=%d\n", count, Count);
    {
        int count = 12; /* This block-scope name hides the outer count. */
        printf("inner count=%d\n", count);
    }
    printf("outer count=%d, Count=%d\n", count, Count);

    puts("-- static storage duration --");
    printf("file-scope total_visits starts at %d\n", total_visits);
    printf("static aggregate defaults: array=%d,%d,%d; record=(%d,%.1f)\n",
           default_values[0], default_values[1], default_values[2],
           default_record.id, default_record.score);
    record_visit();
    record_visit();
    return 0;
}
