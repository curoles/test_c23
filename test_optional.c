#include <assert.h>
#include "mydefines.h"

typedef optional_type(int) optional_int_t;

static
optional_int_t test_optional_val(int i) {
    return (i > 0)? (optional_int_t){.present=true, .value=i} :
                    (optional_int_t){.present=false};
}

typedef error_value_type(float, int) err_float_t;

err_float_t test_err_value(int i) {
    return (i > 0)? (err_float_t){.value=(float)i, .iserr=false} :
                    (err_float_t){.error=7, .iserr=true};
}

int test_optional(void)
{
    {
    __auto_type a = test_optional_val(10);
    assert(a.present && a.value == 10);
    a = test_optional_val(-10);
    assert(!a.present);
    }

    {
    __auto_type a = test_err_value(10);
    assert(!a.iserr && a.value == 10.0);
    a = test_err_value(-10);
    assert(a.iserr && a.error==7);
    }

    return 0;
}
