/*
 * Demonstrate compound assignments and their results with bounded integer values.
 * Input: none. Compile as C11 and run.
 */
#if defined(__MINGW32__) && !defined(__USE_MINGW_ANSI_STDIO)
#define __USE_MINGW_ANSI_STDIO 1
#endif

#include <stddef.h>
#include <stdio.h>

int main(void)
{
    int value = 5;
    value += 3;
    printf("+= gives %d\n", value);
    value *= 2;
    printf("*= gives %d\n", value);

    value = 20;
    value -= 8;
    printf("-= gives %d\n", value);
    value /= 3;
    printf("/= gives %d\n", value);
    value %= 3;
    printf("%%= gives %d\n", value);

    value = 2;
    value <<= 2;
    printf("<<= gives %d\n", value);
    value >>= 1;
    printf(">>= gives %d\n", value);
    value &= 6;
    printf("&= gives %d\n", value);
    value ^= 1;
    printf("^= gives %d\n", value);
    value |= 8;
    printf("|= gives %d\n", value);

    int items[] = {1, 2};
    size_t index = 0;
    items[index++] += 5;
    printf("left side evaluated once: index=%zu, updated item=%d\n", index, items[0]);
    /* Shift operands and results are chosen to stay in range; no undefined cases are run. */
    return 0;
}