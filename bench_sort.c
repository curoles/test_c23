#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "mydefines.h"

#define _ARRAY_TYPE int64_t
#define _ARRAY_TYPE_NAME int64
#include "array.inc.h"

typedef struct timespec timespec;

static
timespec time_diff(timespec start, timespec end)
{
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}

static
void bench_insertion_sort(unsigned int len, unsigned int times)
{
    auto_free int64_smart_array_t* a = int64_smart_array_heap_new(len, malloc);

    timespec time1, time2;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (unsigned int n = 0; n < times; ++n)
    {
        for (unsigned int i = 0; i < a->len; ++i) {
            a->data[i] = a->len - i;
        }
        int64_smart_array_insertion_sort(a);
        assert(a->data[len-2] <= a->data[len-1]);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    timespec t = time_diff(time1, time2);

    printf("Insertion Sort\n  time=%lu.%lu\n", t.tv_sec, t.tv_nsec);

    for (unsigned int i = 1; i < a->len; ++i) {
        //printf("a[%u]:%d <= a[%u]:%d\n", i-1, e->data[i-1], i, e->data[i]);
        assert(a->data[i-1] <= a->data[i]);
    }
}

static
void bench_bubble_sort(unsigned int len, unsigned int times)
{
    auto_free int64_smart_array_t* a = int64_smart_array_heap_new(len, malloc);

    timespec time1, time2;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (unsigned int n = 0; n < times; ++n)
    {
        for (unsigned int i = 0; i < a->len; ++i) {
            a->data[i] = a->len - i;
        }
        int64_smart_array_bubble_sort(a);
        assert(a->data[len-2] <= a->data[len-1]);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    timespec t = time_diff(time1, time2);

    printf("Bubble Sort\n  time=%lu.%lu\n", t.tv_sec, t.tv_nsec);

    for (unsigned int i = 1; i < a->len; ++i) {
        //printf("a[%u]:%d <= a[%u]:%d\n", i-1, e->data[i-1], i, e->data[i]);
        assert(a->data[i-1] <= a->data[i]);
    }
}

static
int int64_compare(const void* pa, const void* pb)
{
    int64_t a = *((int64_t*)pa);
    int64_t b = *((int64_t*)pb);

    return ( (a < b)? -1 : ( (a == b)? 0 : 1) );
}

static
void bench_lib_qsort(unsigned int len, unsigned int times)
{
    auto_free int64_smart_array_t* a = int64_smart_array_heap_new(len, malloc);

    timespec time1, time2;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (unsigned int n = 0; n < times; ++n)
    {
        for (unsigned int i = 0; i < a->len; ++i) {
            a->data[i] = a->len - i;
        }
        qsort(a->data, a->len, sizeof(int64_t), int64_compare);
        assert(a->data[len-2] <= a->data[len-1]);
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

    timespec t = time_diff(time1, time2);

    printf("Library Q Sort\n  time=%lu.%lu\n", t.tv_sec, t.tv_nsec);

    for (unsigned int i = 1; i < a->len; ++i) {
        //printf("a[%u]:%d <= a[%u]:%d\n", i-1, e->data[i-1], i, e->data[i]);
        assert(a->data[i-1] <= a->data[i]);
    }
}

int main(void)
{
    constexpr unsigned int len = 10000;
    constexpr unsigned int times = 200;

    bench_insertion_sort(len, times);
    bench_bubble_sort(len, times);
    bench_lib_qsort(len, times);

    return 0;
}
