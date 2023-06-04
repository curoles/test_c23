#include <stdio.h>
#include "smart_string.h"

int test_string(void)
{
    {
        SMART_STRING(s, 100);
    }

    {
        SMART_STRING_FROM(s, "Hello you!");
        puts(smart_string_get(&s));

        smart_string_append(&s, " And you!");
        puts(smart_string_get(&s));
    }

    return 0;
}
