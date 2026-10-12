/*
 * Demonstrate scanf-family formats and safe line-based numeric parsing.
 * Input: three lines: a name, a nonnegative integer, and a nonnegative finite number.
 * Compile as C11, run, then enter those three values.
 */
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* scanf %f expects float * and %lf expects double *; printf %f uses double. */
static void demonstrate_scanf_formats(void)
{
    const char sample[] = "-12 42 3.5 6.25 Z word";
    int signed_value = 0;
    unsigned int unsigned_value = 0;
    float single_value = 0.0f;
    double double_value = 0.0;
    char character = '\0';
    char word[20] = "";
    int assigned = sscanf(sample, "%d %u %f %lf %c %19s",
                          &signed_value, &unsigned_value, &single_value,
                          &double_value, &character, word);

    printf("assigned=%d: int=%d unsigned=%u float=%.1f double=%.2f char=%c word=%s\n",
           assigned, signed_value, unsigned_value, single_value,
           double_value, character, word);

    char raw_character = '\0';
    char first_nonspace = '\0';
    int raw_result = sscanf(" A", "%c", &raw_character);
    int spaced_result = sscanf(" A", " %c", &first_nonspace);
    printf("%%c without leading space reads whitespace=%s; with space reads=%c\n",
           raw_result == 1 && raw_character == ' ' ? "yes" : "no",
           spaced_result == 1 ? first_nonspace : '?');

    int left = 0;
    int right = 0;
    int partial = sscanf("17 nope", "%d %d", &left, &right);
    printf("matching failure after one assignment returns %d\n", partial);

    int year = 0;
    int suppressed = sscanf("2026-10", "%d-%*d", &year);
    printf("suppressed conversion returns %d assignment; year=%d\n", suppressed, year);

    int empty_value = 0;
    int empty_result = sscanf("", "%d", &empty_value);
    printf("empty input before conversion returns EOF=%s\n",
           empty_result == EOF ? "yes" : "no");

    char first_word[20] = "";
    (void)sscanf("red blue", "%19s", first_word);
    printf("bounded %%s stops at whitespace: %s\n", first_word);
    /* A field width counts input characters; multibyte text may need more storage bytes. */
    /* scanf skips leading space for most conversions, but not for %c, %[ or %n. */
    /* scanf_s is optional Annex K; numeric input outside the target range is unsafe. */
}

static int read_line(char *buffer, int capacity)
{
    if (capacity < 2 || fgets(buffer, capacity, stdin) == NULL) {
        return 0;
    }

    size_t length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[--length] = '\0';
    } else {
        int ch = getchar();
        if (ch != '\n' && ch != EOF) {
            while (ch != '\n' && ch != EOF) {
                ch = getchar();
            }
            return 0;
        }
    }
    if (length > 0 && buffer[length - 1] == '\r') {
        buffer[--length] = '\0';
    }
    return 1;
}

static int parse_int_line(const char *text, int *result)
{
    char *end;
    errno = 0;
    long value = strtol(text, &end, 10);

    if (errno == ERANGE || end == text) {
        return 0;
    }
    while (isspace((unsigned char)*end)) {
        ++end;
    }
    if (*end != '\0' || value < INT_MIN || value > INT_MAX) {
        return 0;
    }
    *result = (int)value;
    return 1;
}

static int parse_finite_nonnegative_double(const char *text, double *result)
{
    char *end;
    errno = 0;
    double value = strtod(text, &end);

    if (errno == ERANGE || end == text) {
        return 0;
    }
    while (isspace((unsigned char)*end)) {
        ++end;
    }
    if (*end != '\0' || !isfinite(value) || value < 0.0) {
        return 0;
    }
    *result = value;
    return 1;
}

int main(void)
{
    char name[64];
    char integer_text[64];
    char decimal_text[64];
    int age;
    double measurement;

    demonstrate_scanf_formats();

    printf("Name (one line): ");
    if (!read_line(name, (int)sizeof name) || name[0] == '\0') {
        fputs("Name is empty or too long.\n", stderr);
        return 1;
    }

    printf("Age (nonnegative integer): ");
    if (!read_line(integer_text, (int)sizeof integer_text) ||
        !parse_int_line(integer_text, &age) || age < 0) {
        fputs("Age is invalid or outside the int range.\n", stderr);
        return 1;
    }

    printf("Measurement (finite, nonnegative number): ");
    if (!read_line(decimal_text, (int)sizeof decimal_text) ||
        !parse_finite_nonnegative_double(decimal_text, &measurement)) {
        fputs("Measurement is invalid or outside the double range.\n", stderr);
        return 1;
    }

    printf("Name: %s\nAge: %d\nMeasurement: %.2f\n", name, age, measurement);
    if (age == INT_MAX) {
        puts("Next age is outside the int range.");
    } else {
        printf("Next age: %d\n", age + 1);
    }
    return 0;
}
