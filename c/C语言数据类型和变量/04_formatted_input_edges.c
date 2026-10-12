/*
 * Demonstrate scanf scansets and %n using fixed, controlled input.
 * Input: none. Compile as C11 and run.
 */
#include <stdio.h>

int main(void)
{
    const char text[] = " ABC";
    char without_space[8] = "";
    char after_space[8] = "";

    int raw_result = sscanf(text, "%7[ABCDEFGHIJKLMNOPQRSTUVWXYZ]", without_space);
    int spaced_result = sscanf(text, " %7[ABCDEFGHIJKLMNOPQRSTUVWXYZ]", after_space);
    printf("%%[ does not skip whitespace: result=%d; leading format space: result=%d, text=%s\n",
           raw_result, spaced_result, after_space);

    int year = 0;
    int month = 0;
    int date_assignments = sscanf("2026-10", "%d-%d", &year, &month);
    printf("literal date assignments=%d; year=%d; month=%d\n",
           date_assignments, year, month);

    int value = 0;
    int consumed = -1;
    int assignments = sscanf("42 rest", "%d%n", &value, &consumed);
    printf("%%n consumed=%d; counted assignments=%d; value=%d\n",
           consumed, assignments, value);
    /* %n stores the consumed-character count but does not increase scanf's return value. */
    /* scanf_s is an optional Annex K interface and is not available on every implementation. */
    return 0;
}