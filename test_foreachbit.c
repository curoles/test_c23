#include <stdio.h>
#include "mydefines.h"

int test_foreachbit(void)
{
    auto_type b = 0b1011000u;
    for_each_bit(b, pos) {
        printf("pos=%u bits=%b\n", pos, b);
    }

    unsigned long c = 0b1001001111110001110001010101010111000001011000ul;
    for_each_bit(c, pos) {
        printf("pos=%u bits=%lb\n", pos, c);
    }

    return 0;
}
