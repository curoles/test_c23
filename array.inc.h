// Author: Igor Lesik 2023

#define PPCAT_NX(a, b) a ## b
#define PPCAT(a, b) PPCAT_NX(a, b)

#define _ARRAY_FN(name) PPCAT(_ARRAY_TYPE_NAME, PPCAT(_array_, name))
#define _SARRAY_FN(name) PPCAT(_ARRAY_TYPE_NAME, PPCAT(_smart_array_, name))

#define _ARRAY_RO(ref_index, size_index) __attribute__ ((access (read_only, ref_index, size_index)))
#define _ARRAY_WO(ref_index, size_index) __attribute__ ((access (write_only, ref_index, size_index)))
#define _ARRAY_RW(ref_index, size_index) __attribute__ ((access (read_write, ref_index, size_index)))

#ifndef _ARRAY_TYPE_EQ
#define _ARRAY_TYPE_EQ(a, b) ({(a) == (b);})
#endif

#ifndef _ARRAY_TYPE_LT
#define _ARRAY_TYPE_LT(a, b) ({(a) < (b);})
#endif

#define _SMART_ARRAY   PPCAT(_ARRAY_TYPE_NAME, _smart_array)
#define _SMART_ARRAY_T PPCAT(_ARRAY_TYPE_NAME, _smart_array_t)

/* Smart_array knows its own length.
 *
 * Example:
 * ```
 * static int_smart_array_t static_b = {3, {4, 5, 6}};
 * assert(static_b.data[0] == 4);
 * assert(static_b.data[1] == 5);
 * assert(static_b.data[2] == 6);
 * static int_smart_array_t static_c = {3, {[0 ... 2]=7}};
 *
 * auto_free int_smart_array_t* heap_a = int_smart_array_heap_new(100, malloc);
 * d = heap_a->data;
 * for (int i = 0; i < heap_a->len; ++i) {d[i]=i;}
 * for (int i = 0; i < heap_a->len; ++i) {assert(d[i] == i);}
 * ```
 */
typedef struct _SMART_ARRAY {
    unsigned int len;
    _ARRAY_TYPE data[];
} _SMART_ARRAY_T;


/* Allocate smart_array on stack.
 *
 * Example:
 * ```
 * int_smart_array_t* stack_a = smart_array_stack_new(int_smart_array_t, int, 100);
 * int* d = stack_a->data;
 * for (int i = 0; i < stack_a->len; ++i) {d[i]=i;}
 * for (int i = 0; i < stack_a->len; ++i) {assert(d[i] == i);}
 * ```
 */
#define smart_array_stack_new(ARRAY, T, alen) \
    ({ \
    typeof (alen) _ARRAY_len = (alen); \
    ARRAY* ptr = (ARRAY*) \
        __builtin_alloca(sizeof(ARRAY) + _ARRAY_len*sizeof(T)); \
    ptr->len = _ARRAY_len; \
    ptr;})

/* Allocate smart_array on heap.
 *
 * Example:
 * ```
 * auto_free int_smart_array_t* heap_a = int_smart_array_heap_new(100, malloc);
 * ```
 */
static inline
FN_ATTR_WARN_UNUSED_RESULT
_SMART_ARRAY_T*
_SARRAY_FN(heap_new)(unsigned int len, void*(*allocator)(long unsigned int)) {
    _SMART_ARRAY_T* ptr = (_SMART_ARRAY_T*)
        allocator(sizeof(_SMART_ARRAY_T) + len*sizeof(_ARRAY_TYPE));
    ptr->len = len;
    return ptr;
}

static inline
FN_ATTR_WARN_UNUSED_RESULT
_SMART_ARRAY_T*
_SARRAY_FN(heap_realloc)(_SMART_ARRAY_T* self, unsigned int len, void*(*reallocator)(void*, long unsigned int)) {
    _SMART_ARRAY_T* ptr = (_SMART_ARRAY_T*)
        reallocator(self, sizeof(_SMART_ARRAY_T) + len*sizeof(_ARRAY_TYPE));
    ptr->len = len;
    return ptr;
}

static inline
_ARRAY_RO(2, 1) FN_ATTR_WARN_UNUSED_RESULT
_ARRAY_TYPE
_ARRAY_FN(get_at)(unsigned int len, const _ARRAY_TYPE a[len], unsigned int pos)
{
#ifdef _ASSERT_H
    assert(pos < len); // access with bounds checking
#endif
    return a[pos];
}

static inline
__attribute__((nonnull(1))) FN_ATTR_WARN_UNUSED_RESULT
_ARRAY_TYPE
_SARRAY_FN(get_at)(const _SMART_ARRAY_T* a, unsigned int pos)
{
    return _ARRAY_FN(get_at)(a->len, a->data, pos);
}

static inline
_ARRAY_WO(2, 1)
void
_ARRAY_FN(set_at)(unsigned int len, _ARRAY_TYPE a[len], unsigned int pos, _ARRAY_TYPE val)
{
#ifdef _ASSERT_H
    assert(pos < len); // access with bounds checking
#endif
    a[pos] = val;
}

static inline
__attribute__((nonnull(1)))
void
_SARRAY_FN(set_at)(_SMART_ARRAY_T* a, unsigned int pos, _ARRAY_TYPE val)
{
    _ARRAY_FN(set_at)(a->len, a->data, pos, val);
}

static inline
_ARRAY_RO(2, 1) FN_ATTR_WARN_UNUSED_RESULT
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
__attribute__((nonnull(1))) FN_ATTR_WARN_UNUSED_RESULT
optional_uint_t
_SARRAY_FN(find)(_SMART_ARRAY_T* a, _ARRAY_TYPE val)
{
    return _ARRAY_FN(find)(a->len, a->data, val);
}

static inline
_ARRAY_RO(2, 1) FN_ATTR_WARN_UNUSED_RESULT
bool
_ARRAY_FN(contains)(unsigned int len, const _ARRAY_TYPE a[len], _ARRAY_TYPE val_to_find)
{
    auto_type pos = _ARRAY_FN(find)(len, a, val_to_find);

    return pos.present;
}

static inline
_ARRAY_RO(2, 1) _ARRAY_RO(3, 1) FN_ATTR_WARN_UNUSED_RESULT
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

static inline
_ARRAY_RO(2, 1) _ARRAY_WO(3, 1) FN_ATTR_RETURNS_NONNULL
_ARRAY_TYPE*
_ARRAY_FN(copy)(unsigned int len, const _ARRAY_TYPE src[len], _ARRAY_TYPE dst[len])
{
    for (unsigned int i = 0; i < len; ++i) {
        dst[i] = src[i];
    }

    return dst;
}

static inline
_ARRAY_RO(2, 1) _ARRAY_WO(3, 1) FN_ATTR_RETURNS_NONNULL
_ARRAY_TYPE*
_ARRAY_FN(memcopy)(unsigned int len, const _ARRAY_TYPE src[len], _ARRAY_TYPE dst[len])
{
    return (_ARRAY_TYPE*) __builtin_memcpy(dst, src, len * sizeof(dst[0]));
}

static inline
_ARRAY_WO(2, 1) FN_ATTR_RETURNS_NONNULL
_ARRAY_TYPE*
_ARRAY_FN(fill)(unsigned int len, _ARRAY_TYPE a[len], _ARRAY_TYPE val)
{
    for (unsigned int i = 0; i < len; ++i) {
        a[i] = val;
    }

    return a;
}

static inline
__attribute__((nonnull(1))) FN_ATTR_RETURNS_NONNULL
_ARRAY_TYPE*
_SARRAY_FN(fill)(_SMART_ARRAY_T* a, _ARRAY_TYPE val)
{
    return _ARRAY_FN(fill)(a->len, a->data, val);
}

static inline
_ARRAY_RW(2, 1) FN_ATTR_RETURNS_NONNULL
_ARRAY_TYPE*
_ARRAY_FN(insertion_sort)(unsigned int len, _ARRAY_TYPE a[len])
{
    unsigned int j;
    _ARRAY_TYPE key;

    for (unsigned int i = 1; i < len; ++i) {
        key = a[i];
        j = i;
 
        /* Shift elements of a[0..i-1], that are GT key, to one position right */
        while (j > 0 && _ARRAY_TYPE_LT(key, a[j - 1])) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = key;
    }

    return a;
}

static inline
__attribute__((nonnull(1))) FN_ATTR_RETURNS_NONNULL
_ARRAY_TYPE*
_SARRAY_FN(insertion_sort)(_SMART_ARRAY_T* a)
{
    return _ARRAY_FN(insertion_sort)(a->len, a->data);
}

#undef _SMART_ARRAY
#undef _SMART_ARRAY_T
#undef _ARRAY_TYPE_EQ
#undef _ARRAY_TYPE_LT
#undef _ARRAY_RO
#undef _ARRAY_WO
#undef _ARRAY_RW
#undef _ARRAY_FN
#undef _SARRAY_FN
#undef PPCAT_NX
#undef PPCAT

#undef _ARRAY_TYPE
#undef _ARRAY_TYPE_NAME
