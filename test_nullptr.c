#include "mydefines.h"

#include <stdio.h>
#include <assert.h>

#define print_type(a) printf("%s\n", _Generic( (a) , \
                                  int : "int", \
                                  long : "long", \
                                  float : "float", \
                                  nullptr_t : "nullptr_t", \
                                  default : "other type"))

int test_nullptr(void)
{
    printf("C version %lu\n", __STDC_VERSION__);

    print_type(nullptr);
    auto_type a = 10;
    print_type(a);

    return 0;
}
