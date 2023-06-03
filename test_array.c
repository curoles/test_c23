#include <assert.h>
#include <stdlib.h>
#include "mydefines.h"

#define _ARRAY_TYPE int
#define _ARRAY_TYPE_NAME int
#include "array.inc.h"


// see: https://gcc.gnu.org/onlinedocs/gcc/Zero-Length.html
int_smart_array_t static_a = {3, {1, 2, 3}};

static
int test_array2(void);

int test_array(void)
{
    assert(static_a.data[0] == 1);
    assert(static_a.data[1] == 2);
    assert(static_a.data[2] == 3);

    static int_smart_array_t static_b = {3, {4, 5, 6}};
    assert(static_b.data[0] == 4);
    assert(static_b.data[1] == 5);
    assert(static_b.data[2] == 6);

    static int_smart_array_t static_c = {3, {[0 ... 2]=7}};
    assert(static_c.data[0] == 7);
    assert(static_c.data[1] == 7);
    assert(static_c.data[2] == 7);

    int_smart_array_t* stack_a = int_smart_array_stack_new(100);
    int* d = stack_a->data;
    for (int i = 0; i < stack_a->len; ++i) {d[i]=i;}
    for (int i = 0; i < stack_a->len; ++i) {assert(d[i] == i);}

    auto_free int_smart_array_t* heap_a = int_smart_array_heap_new(100, malloc);
    d = heap_a->data;
    for (int i = 0; i < heap_a->len; ++i) {d[i]=i;}
    for (int i = 0; i < heap_a->len; ++i) {assert(d[i] == i);}

    test_array2();

    return 0;
}

static
int test_array2(void)
{
    int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto_type pos = int_array_find(10, a, 7);
    assert(pos.present && pos.value == 7);

    pos = int_array_find(10, a, 77);
    assert(!pos.present);

    assert(int_array_contains(10, a, 5));
    assert(!int_array_contains(10, a, 55));

    assert(int_array_equal(10, a, a));

    int b[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    assert(!int_array_equal(10, a, b));

    int c[10] = {};
    int_array_copy(10, a, c);
    assert(int_array_equal(10, a, c));
    int_array_memcopy(10, b, c);
    assert(!int_array_equal(10, a, c));
    assert(int_array_equal(10, b, c));

    int_array_fill(10, c, 777);
    assert(int_array_get_at(10, c, 3) == 777);
    int_array_set_at(10, c, 3, 888);
    assert(int_array_get_at(10, c, 3) == 888);

    auto_free int_smart_array_t* d = int_smart_array_heap_new(100, malloc);
    int_smart_array_set_at(d, 33, 999);
    assert(int_smart_array_get_at(d, 33) == 999);

    pos = int_smart_array_find(d, 999);
    assert(pos.present && pos.value == 33);

    return 0;
}
