#include <stdio.h>
#include "mydefines.h"

typedef transparent_union(Num64) {
    unsigned long i;
    double f;
} Num64;

static void transunion_print(Num64 n) {
    printf("%lx %f\n", n.i, n.f);
}

int test_transparent_union(void)
{
    transunion_print(1.0);
    transunion_print(0x4000000000000000UL);

    return 0;
}

