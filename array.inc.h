
#define PPCAT_NX(a, b) a ## b
#define PPCAT(a, b) PPCAT_NX(a, b)

#define _ARRAY_FN(name) PPCAT(_ARRAY_TYPE_NAME, PPCAT(_array_, name))

#define _ARRAY_RO(ref_index, size_index) __attribute__ ((access (read_only, ref_index, size_index)))

#ifndef _ARRAY_TYPE_EQ
#define _ARRAY_TYPE_EQ(a, b) ({(a) == (b);})
#endif

static inline
_ARRAY_RO(2, 1)
optional_uint_t
_ARRAY_FN(find)(unsigned int len, const _ARRAY_TYPE a[len], _ARRAY_TYPE val_to_find)
{
    optional_uint_t pos = {.present = false};
    for (unsigned int i = 0; i < len; ++i) {
        if (_ARRAY_TYPE_EQ(a[i], val_to_find)) {
            pos.present = true;
            pos.value = i;
            break;
        }
    }

    return pos;
}

static inline
_ARRAY_RO(2, 1)
bool
_ARRAY_FN(contains)(unsigned int len, const _ARRAY_TYPE a[len], _ARRAY_TYPE val_to_find)
{
    auto_type pos = _ARRAY_FN(find)(len, a, val_to_find);

    return pos.present;
}

static inline
_ARRAY_RO(2, 1) _ARRAY_RO(3, 1)
bool
_ARRAY_FN(equal)(unsigned int len, const _ARRAY_TYPE a[len], const _ARRAY_TYPE b[len])
{
    bool equal = true;

    for (unsigned int i = 0; i < len; ++i) {
        if (!_ARRAY_TYPE_EQ(a[i], b[i])) {
            equal = false;
            break;
        }
    }

    return equal;
}

#undef _ARRAY_TYPE_EQ
#undef _ARRAY_RO
#undef _ARRAY_FN
#undef PPCAT_NX
#undef PPCAT

#undef _ARRAY_TYPE
#undef _ARRAY_TYPE_NAME
