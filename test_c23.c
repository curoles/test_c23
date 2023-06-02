#include <stdio.h>
#include <assert.h>
#include "mydefines.h"

typedef int(*test_fn_t)(void);

extern int test_nullptr(void);
extern int test_cleanup(void);
extern int test_transparent_union(void);
extern int test_enum(void);
extern int test_optional(void);
extern int test_foreachbit(void);
extern int test_returnif(void);
extern int test_arraylen(void);
extern int test_array(void);
extern int test_constexpr(void);
extern int test_immutable(void);
extern int test_offsetof(void);

static test_fn_t tests[] = {
    test_nullptr,
    test_cleanup,
    test_transparent_union,
    test_enum,
    test_optional,
    test_foreachbit,
    test_returnif,
    test_arraylen,
    test_array,
    test_constexpr,
    test_immutable,
    test_offsetof,
    0
};

int main(void)
{
    test_fn_t* test = tests;
    while (*test) {
        printf("\nTesting...\n");
        (*test)();
        ++test;
    }

    return 0;
}
