#include "c23defines.h"
#include <assert.h>

int test_returnif(void)
{
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

    return 0;
}
