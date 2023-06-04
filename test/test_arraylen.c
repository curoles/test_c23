#include <assert.h>
#include "c23defines.h"

int test_arraylen(void)
{
    int a[10] = {};
    assert(fixlen_array_len(a) == 10);
    static_assert(fixlen_array_len(a) == 10);

    float b[] = {1, 2.0, 3.};
    assert(fixlen_array_len(b) == 3);
    static_assert(fixlen_array_len(b) == 3);

    return 0;
}
