#include <assert.h>
#include "mydefines.h"

__attribute__ ((const))
static inline
int square(int a) {
    return a*a;
}

int test_constexpr(void)
{
    constexpr int i2 = 3*3;//square(3);
    static_assert(i2 == 9);

    // automatic storage duration
    int *p = (int []){2, 4}; // p points to the first element of an array of two ints
    assert(p[1] == 4);
    const int *q = &(const int){77};
    assert(*q == 77);

    // https://developers.redhat.com/articles/2023/05/04/new-c-features-gcc-13
    // https://thephd.dev/c23-is-coming-here-is-what-is-on-the-menu
    return 0;
}
