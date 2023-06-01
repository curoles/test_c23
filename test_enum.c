#include "mydefines.h"

typedef_enum(Rgb, unsigned char) {
    RED, GREEN/*=256 error*/, BLUE/*=-1 error */
} Rgb;

int test_enum(void)
{
    return 0;
}
