#include <assert.h>
#include <stdlib.h>
#include "mydefines.h"

typedef smart_array(int) array_int_t;

// see: https://gcc.gnu.org/onlinedocs/gcc/Zero-Length.html
array_int_t static_a = {3, {1, 2, 3}};


int test_array(void)
{
    assert(static_a.data[0] == 1);
    assert(static_a.data[1] == 2);
    assert(static_a.data[2] == 3);

    static array_int_t static_b = {3, {4, 5, 6}};
    assert(static_b.data[0] == 4);
    assert(static_b.data[1] == 5);
    assert(static_b.data[2] == 6);

    array_int_t* stack_a = smart_array_stack_new(int, 100);
    int* d = stack_a->data;
    for (int i = 0; i < stack_a->len; ++i) {d[i]=i;}
    for (int i = 0; i < stack_a->len; ++i) {assert(d[i] == i);}

    auto_free array_int_t* heap_a = smart_array_heap_new(int, 100, malloc);
    d = heap_a->data;
    for (int i = 0; i < heap_a->len; ++i) {d[i]=i;}
    for (int i = 0; i < heap_a->len; ++i) {assert(d[i] == i);}

    return 0;
}
