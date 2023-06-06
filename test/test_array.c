#if 1
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "c23defines.h"

#define _ARRAY_DEBUG
#define _ARRAY_TYPE int
#define _ARRAY_TYPE_NAME int
#include "array.inc.h"
#else // check that we included all needed headers into ours
#define _ARRAY_TYPE int
#define _ARRAY_TYPE_NAME int
#include "array.inc.h"

#include "c23defines.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#endif


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
    assert(static_b.data[0] == 4); assert(int_smart_array_get_at(&static_b, 0) == 4);
    assert(static_b.data[1] == 5);
    assert(static_b.data[2] == 6);

    static int_smart_array_t static_c = {3, {[0 ... 2]=7}};
    assert(static_c.data[0] == 7);
    assert(static_c.data[1] == 7);
    assert(static_c.data[2] == 7);

    printf("sizeof(int_smart_array_t)=%lu\n", sizeof(int_smart_array_t));
    //assert(sizeof(int_smart_array_t) == sizeof(unsigned int));
    assert(sizeof(int_smart_array_t) == _SMART_ARRAY_ALIGN);

    int_smart_array_t* stack_a = smart_array_stack_new(int_smart_array_t, int, 100);
    assert(stack_a != nullptr);
    int* d = stack_a->data;
    assert(stack_a->len == 100);
    printf("stack allocated array data pointer %p\n", d);
    assert(((size_t)d & (_SMART_ARRAY_ALIGN - 1)) == 0);
    for (unsigned int i = 0; i < stack_a->len; ++i) {d[i]=i;}
    for (unsigned int i = 0; i < stack_a->len; ++i) {assert(d[i] == (int)i);}

    auto_free int_smart_array_t* heap_a = int_smart_array_heap_new(100);
    assert(heap_a != nullptr);
    assert(heap_a->len == 100);
    d = heap_a->data;
    printf("heap allocated array data pointer %p\n", d);
    assert(((size_t)d & (_SMART_ARRAY_ALIGN - 1)) == 0);
    for (unsigned int i = 0; i < heap_a->len; ++i) {d[i]=i;}
    for (unsigned int i = 0; i < heap_a->len; ++i) {assert(d[i] == (int)i);}

    test_array2();

    return 0;
}

static
int test_array2(void)
{
    ATTR_SMART_ARRAY_ALIGNED
    int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto_type pos = int_array_find(10, a, 7);
    assert(pos.present && pos.value == 7);

    pos = int_array_find(10, a, 77);
    assert(!pos.present);

    assert(int_array_contains(10, a, 5));
    assert(!int_array_contains(10, a, 55));

    assert(int_array_equal(10, a, a));

    ATTR_SMART_ARRAY_ALIGNED
    int b[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    assert(!int_array_equal(10, a, b));

    ATTR_SMART_ARRAY_ALIGNED
    int c[10] = {};

    int_array_copy(10, a, c);
    assert(int_array_equal(10, a, c));
    int_array_memcopy(10, b, c);
    assert(!int_array_equal(10, a, c));
    assert(int_array_equal(10, b, c));

    int_array_fill(10, c, 777); // XXX c must be aligned by _SMART_ARRAY_ALIGN
    assert(int_array_get_at(10, c, 3) == 777);
    int_array_set_at(10, c, 3, 888);
    assert(int_array_get_at(10, c, 3) == 888);

    auto_free int_smart_array_t* d = int_smart_array_heap_new(100);
    int_smart_array_fill(d, 111);
    int_smart_array_set_at(d, 33, 999);
    assert(int_smart_array_get_at(d, 33) == 999);

    pos = int_smart_array_find(d, 999);
    assert(pos.present && pos.value == 33);

    auto_free int_smart_array_t* ee = int_smart_array_heap_new(100);
    ee->data[5] = 55;
    auto_free void* misal1 = malloc(10*64+1);
    auto_free void* misal2 = malloc(21*64+2);
    auto_free void* misal3 = malloc(23*64+3);
    auto_free int_smart_array_t* e = int_smart_array_heap_realloc(ee, 1000);
    printf("heap re-allocated array data pointer %p\n", e->data);
    assert(((size_t)e->data & (_SMART_ARRAY_ALIGN - 1)) == 0);
    assert(e->data[5] == 55);

    for (unsigned int i = 0; i < e->len; ++i) {
        e->data[i] = e->len - i;
    }

    int_smart_array_insertion_sort(e);
    for (unsigned int i = 1; i < e->len; ++i) {
        //printf("a[%u]:%d <= a[%u]:%d\n", i-1, e->data[i-1], i, e->data[i]);
        assert(e->data[i-1] < e->data[i]);
    }

    for (unsigned int i = 0; i < e->len; ++i) {
        e->data[i] = e->len - i;
    }

    int_smart_array_bubble_sort(e);
    for (unsigned int i = 1; i < e->len; ++i) {
        //printf("a[%u]:%d <= a[%u]:%d\n", i-1, e->data[i-1], i, e->data[i]);
        assert(e->data[i-1] < e->data[i]);
    }

    assert(misal1 != nullptr);
    assert(misal2 != nullptr);
    assert(misal3 != nullptr);

    return 0;
}
