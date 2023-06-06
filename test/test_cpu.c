#include "cpu.h"

#include <stdio.h>
#include <omp.h>

int test_cpu(void)
{
    (void)SMARTARR_L1_DCACHE_SIZE;

    double start_time = omp_get_wtime();
    double time = omp_get_wtime() - start_time;
 
    printf("%0.6f s\n", time);

    return 0;
}
