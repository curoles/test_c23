#include <stddef.h>
#include <assert.h>
#include "c23defines.h"

struct A {
    struct B {} b;
    struct C {} c;
    struct D {
        struct E {} e;
    } d;
};

int test_offsetof(void)
{
    struct A a;

    struct B* b = &a.b;
    struct C* c = &a.c;
    struct D* d = &a.d;
    struct E* e = &d->e;

    assert(container_of(b, struct A, b) == &a);
    assert(container_of(c, struct A, c) == &a);
    assert(container_of(d, struct A, d) == &a);
    assert(container_of(e, struct D, e) ==  d);

    return 0;
}
