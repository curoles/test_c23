#include <stdio.h>
#include <assert.h>
#include "smart_string.h"

int test_string(void)
{
    {
        SMART_STRING(s, 100);
        smart_string_append(&s, "Ai Ai!");
        puts(smart_string_get(&s));
        assert(smart_string_compare(&s, "Aii Ai!") == 0);
        assert(smart_string_equal(&s, "Aii Ai!"));
    }

    {
        SMART_STRING_FROM(s, "Hello you!");
        puts(smart_string_get(&s));

        smart_string_append(&s, " And you!");
        puts(smart_string_get(&s));
        assert(smart_string_equal(&s, "Hello you! And you!"));

        assert(smart_string_find_first_char(&s, '!'));
        assert(smart_string_find_first_char(&s, '?') == nullptr);

        puts(smart_string_find_first_substring(&s, "And"));
        assert(smart_string_contains(&s, "And"));
    }

    return 0;
}
