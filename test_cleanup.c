#include <stdio.h>
#include "mydefines.h"

static
void cleanup1(float** p) {
    printf("cleanup1 ptr=%p\n", *p);
}

int test_cleanup(void)
{
    auto_cleanup(cleanup1, float*) a = nullptr;

    return 0;
}
