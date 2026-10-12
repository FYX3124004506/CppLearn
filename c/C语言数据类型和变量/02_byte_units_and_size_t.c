/*
 * Demonstrate C byte units, size_t, and the difference between an array and a pointer.
 * Input: none. Compile as C11 and run.
 */
#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <limits.h>
#include <stddef.h>
#include <stdio.h>

int main(void)
{
    int values[] = {10, 20, 30, 40};
    int *pointer = values;
    size_t array_bytes = sizeof values;
    size_t element_bytes = sizeof values[0];
    size_t element_count = array_bytes / element_bytes;
    int probe = 4;
    size_t unevaluated_size = sizeof(probe++);

    printf("sizeof(char)=%zu; CHAR_BIT=%d bits per C byte\n", sizeof(char), CHAR_BIT);
    printf("sizeof(int)=%zu; sizeof probe=%zu\n", sizeof(int), sizeof probe);
    int rank_sizes_nondecreasing =
        sizeof(short) <= sizeof(int) && sizeof(int) <= sizeof(long) &&
        sizeof(long) <= sizeof(long long);
    printf("integer type sizes nondecreasing=%s\n",
           rank_sizes_nondecreasing ? "yes" : "no");
    printf("array bytes=%zu; element bytes=%zu; elements=%zu\n",
           array_bytes, element_bytes, element_count);
    printf("pointer bytes=%zu; sizeof *pointer=%zu; first value=%d\n",
           sizeof pointer, sizeof *pointer, *pointer);
    printf("sizeof(probe++)=%zu; probe remains %d\n", unevaluated_size, probe);
    /* sizeof(char) is one C byte; CHAR_BIT shows how many bits that byte contains. */
    /* Exact sizes vary by implementation. sizeof a type name requires parentheses. */
    return 0;
}