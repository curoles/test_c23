#include "mydefines.h"

typedef int(*test_fn_t)(void);
extern int test_nullptr(void);

static test_fn_t tests[] = {
    test_nullptr,
    0
};

#include <stdio.h>
#include <assert.h>
//#include <stdlib.h>

static
void cleanup1(float** p) {printf("cleanup1 ptr=%p\n", *p);}

typedef transparent_union(Num64) {unsigned long i; double f;} Num64;

static void transunion_print(Num64 n) {
    printf("%lx %f\n", n.i, n.f);
}

void test_transunion(void) {
    transunion_print(1.0);
    transunion_print(0x4000000000000000UL);
}

typedef_enum(E23, unsigned char) {
    RED, GREEN/*=256 error*/, BLUE/*=-1 error */
} E23;

typedef optional_type(int) optional_int_t;
optional_int_t test_optional(int i) {
    return (i > 0)? (optional_int_t){.present=true, .value=i} :
                    (optional_int_t){.present=false};
}

typedef error_value_type(float, int) err_float_t;
err_float_t test_err_value(int i) {
    return (i > 0)? (err_float_t){.value=(float)i, .iserr=false} :
                    (err_float_t){.error=7, .iserr=true};
}

int main(void)
{
    test_fn_t* test = tests;
    while (*test) {
        printf("\nTesting...\n");
        (*test)();
        ++test;
    }



    auto_type b = 0b1011000u;
    for_each_bit(b, pos) {
        printf("pos=%u bits=%b\n", pos, b);
    }

    unsigned long c = 0b1001001111110001110001010101010111000001011000ul;
    for_each_bit(c, pos) {
        printf("pos=%u bits=%lb\n", pos, c);
    }

    auto_cleanup(float*, cleanup1) d = nullptr;

    test_transunion();

    int x = 15;
    enum Size {SMALL, MEDIUM, BIG};
    enum Size sz = return_if (x < 10) {
        SMALL;
    }
    return_else_if (x < 20) {
        MEDIUM;
    }
    return_else {
        BIG;
    }));
    assert(sz == MEDIUM);

    {
    __auto_type a = test_optional(10);
    assert(a.present && a.value == 10);
    a = test_optional(-10);
    assert(!a.present);
    }

    {
    __auto_type a = test_err_value(10);
    assert(!a.iserr && a.value == 10.0);
    a = test_err_value(-10);
    assert(a.iserr && a.error==7);
    }

    {
        int a[10] = {};
        assert(fixlen_array_num_elements(a) == 10);
        static_assert(fixlen_array_num_elements(a) == 10);
        float b[] = {1, 2.0, 3.};
        assert(fixlen_array_num_elements(b) == 3);
        static_assert(fixlen_array_num_elements(b) == 3);
    }

    /*{
        void* aaa = nullptr;
        free(aaa);
    }*/

    return 0;
}


