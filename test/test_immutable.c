#include <assert.h>
#include "c23defines.h"

typedef struct S {
    int i;
    char c;
    float f;
} S;

constexpr S const_s = {7, 'a', 123.456f};

// https://developers.redhat.com/articles/2023/05/04/new-c-features-gcc-13
// https://thephd.dev/c23-is-coming-here-is-what-is-on-the-menu
static
FN_ATTR_CONST
const S* get_immutable_instance() {
    // constexpr does NOT work, object is created on stack,
    // valdgrind catches it
    const S* s = &(/*constexpr*/static S){8, 'b', 3.456f};
    return s;
}

static
FN_ATTR_CONST
S* get_instance() {
    S* s = &(static S){11, 'c', 3.8f};
    return s;
}

int test_immutable(void)
{
    //const_s.i = 3; // error: assignment of member ‘i’ in read-only object
    static_assert(const_s.i == 7);
    assert(const_s.i == 7);

    const S* p = get_immutable_instance();
    (void)p;
    //p->i=9; //error: assignment of member ‘i’ in read-only object
    assert(p->i == 8);
    assert(p->f == 3.456f);

    /*const*/ S* p2 = (S*)get_immutable_instance();
    (void)p2;
    p2->i=9;
    assert(p2->i == 9); // XXX problem!
    assert(p->i == 9);
    assert(p2->f == 3.456f);

    S* p3 = get_instance();
    assert(p3->i == 11);
    assert(p3->f == 3.8f);
    p3->i=123;
    S* p4 = get_instance();
    assert(p4->i == 123);

    return 0;
}
