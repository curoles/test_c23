#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "c23defines.h"

#include "bench.h"

#define _SMART_ARRAY_ALIGN 64
#define _ARRAY_TYPE int64_t
#define _ARRAY_TYPE_NAME int64
#include "array.inc.h"

#define _ARRAY_TYPE int64_t
#define _ARRAY_TYPE_NAME xint64
#undef _SMART_ARRAY_ALIGN
#define _SMART_ARRAY_ALIGN 8
#include "array.inc.h"

//static
void bench1(unsigned int len, unsigned int times)
{
    auto_free int64_smart_array_t* a = int64_smart_array_heap_new(len);
    auto_free int64_smart_array_t* b = int64_smart_array_heap_new(len);
    auto_free int64_smart_array_t* c = int64_smart_array_heap_new(len);
    int64_smart_array_random_sequence(a);
    int64_smart_array_random_sequence(b);
    int64_smart_array_fill(c, 0);

    printf("Alignment 64: %8p ", a->data); fflush(0);

    timespec time1, time2;

    // warm up
    int64_array_add(len, a->data, b->data, c->data);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (unsigned int n = 0; n < times; ++n)
    {
        int64_smart_array_fill(c, 0);
        int64_array_add(len, a->data, b->data, c->data);
        for (unsigned int i = 0; i < len; ++i) {
            assert(c->data[i] == (a->data[i] + b->data[i]));
        }
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    double tf = time_diff(time1, time2);

    printf("%10.8f\n", tf);
}

//static
void bench2(unsigned int len, unsigned int times)
{
    auto_free xint64_smart_array_t* a = xint64_smart_array_heap_new(len);
    auto_free xint64_smart_array_t* b = xint64_smart_array_heap_new(len);
    auto_free xint64_smart_array_t* c = xint64_smart_array_heap_new(len);
    xint64_smart_array_random_sequence(a);
    xint64_smart_array_random_sequence(b);
    xint64_smart_array_fill(c, 0);

    printf("Alignment  8: %8p ", a->data); fflush(0);

    timespec time1, time2;

    // warm up
    xint64_array_add(len, a->data, b->data, c->data);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (unsigned int n = 0; n < times; ++n)
    {
        xint64_smart_array_fill(c, 0);
        xint64_array_add(len, a->data, b->data, c->data);
        for (unsigned int i = 0; i < len; ++i) {
            assert(c->data[i] == (a->data[i] + b->data[i]));
        }
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    double tf = time_diff(time1, time2);

    printf("%10.8f\n", tf);
}


int main(void)
{
    constexpr unsigned int len = 1024*1024*16;
    constexpr unsigned int times = 100;

    bench1(len, times);
    auto_free void* misalign_next = malloc(11*64+1);
    bench2(len, times);

    assert(misalign_next != nullptr);

    return 0;
}
