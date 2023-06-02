#pragma once

#define STDC17 201710L

#if __STDC_VERSION__ <= STDC17
    //#pragma message "use C11"
    #define nullptr NULL
    #define nullptr_t void*
    #define auto_type __auto_type
#else
    //#pragma message "use C23"
    #define auto_type auto
#endif

#define FN_ATTR_CONST __attribute__ ((const))

/* Macro to define GCC extention "transparent union".
 *
 * Example:
 * ```
 * typedef transparent_union(Num64) {unsigned long i; double f;} Num64;
 * void transunion_print(Num64 n) {printf("%lx %f\n", n.i, n.f);}
 * transunion_print(1.0); // output: 3ff0000000000000 1.000000
 * transunion_print(0x4000000000000000UL); // output: 4000000000000000 2.000000
 * ```
 */
#define transparent_union(name) union __attribute__((__transparent_union__)) name

/* C23 enum with specific underlying type.
 *
 * Example:
 * ```
 * typedef_enum(Rgb, unsigned char) {
 *   RED, GREEN=256 error, BLUE=-1 error
 * } Rgb;
 * ```
 */
#define typedef_enum(name, underlying_type) \
    typedef enum name : underlying_type

/* Macro to define variable with auto cleanup function when its lefetime is over.
 *
 * Example:
 * ```
 * void cleanup1(float** p) {printf("cleanup1 ptr=%p\n", *p);}
 * auto_cleanup(cleanup1, float*) a = nullptr;
 * ```
 */
#define auto_cleanup(fn, T) __attribute__((cleanup(fn))) T

#if defined _STDLIB_H
/* To be used as `__attribute__((cleanup(cleanup_free)))`
 *
 */
static inline void cleanup_free(void* p) {
    free(*(void**)p);
}
#endif

/* Automatically free heap allocated memory.
 *
 * Example:
 * ```
 * auto_free array_int_t* heap_a = smart_array_heap_new(int, 100, malloc);
 * ```
 */
#define auto_free __attribute__((cleanup(cleanup_free)))


/* Count number of trailing zero bits in an integer.
 *
 */
#define ctz(x) _Generic( (x), \
                   unsigned int: __builtin_ctz, \
                   unsigned long: __builtin_ctzl, \
                   default: __builtin_ctz \
                )(x)

/* For loop over each set bit in an integer.
 *
 * Example:
 * ```
 *   __auto_type b = 0b1011000u;
 *   for_each_bit(b, pos) {
 *       printf("pos=%u bits=%b\n", pos, b);
 *   }
 * ```
 * pos=3 bits=1011000
 * pos=4 bits=1010000
 * pos=6 bits=1000000
 */
#define for_each_bit(bits, pos) \
    for (unsigned int pos = ctz(bits); bits; \
        bits &= bits - 1, pos = ctz(bits))

/* IF-ELSE statement that returns a value. 
 *
 * A compound statement enclosed in parentheses may appear as an expression in GNU C.
 *
 * Example:
 * ```
 * int x = 15;
 * enum Size {SMALL, MEDIUM, BIG};
 * enum Size sz = return_if (x < 10) {
 *     SMALL;
 * }
 * return_else_if (x < 20) {
 *     MEDIUM;
 * }
 * return_else {
 *     BIG;
 * }));
 * assert(sz == MEDIUM);
 * ```
 */
#define return_if(cond) (cond) ? (
#define return_else ):(
#define return_else_if(cond) ):( (cond) ? (

/* Macro to define Optional value type.
 *
 * Example:
 * ```
 * typedef optional_type(int) optional_int_t;
 * optional_int_t test_optional(int i) {
 *   return (i > 0)? (optional_int_t){.present=true, .value=i} :
 *                   (optional_int_t){.present=false};
 * }
 *
 * __auto_type a = test_optional(10);
 * assert(a.present && a.value == 10);
 * a = test_optional(-10);
 * assert(!a.present);
 * ```
 */
#define optional_type(T) struct { T value; bool present; }

/* Macro to define value type with an error.
 *
 * Example:
 * ```
 * typedef error_value_type(float, int) err_float_t;
 * err_float_t test_err_value(int i) {
 *   return (i > 0)? (err_float_t){.value=(float)i, .iserr=false} :
 *                   (err_float_t){.error=7, .iserr=true};
 * }
 *
 * __auto_type a = test_err_value(10);
 * assert(!a.iserr && a.value == 10.0);
 * a = test_err_value(-10);
 * assert(a.iserr && a.error==7);
 * ```
 */
#define error_value_type(T, E) struct { union {T value; E error;}; bool iserr; }

#define container_of(ptr, type, member) ({ \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - offsetof(type,member) );})

/* Return number of elements in fixed length array.
 *
 * Example:
 * ```
 * int a[10] = {};
 * assert(fixlen_array_len(a) == 10);
 * static_assert(fixlen_array_len(a) == 10);
 * float b[] = {1, 2.0, 3.};
 * assert(fixlen_array_len(b) == 3);
 * static_assert(fixlen_array_len(b) == 3);
 * ```
 */
#define fixlen_array_len(a) ((sizeof(a)) / (sizeof(a[0])))

/* Smart_array knows its own length.
 *
 * Example:
 * ```
 * typedef smart_array(int) array_int_t;
 *
 * static array_int_t static_b = {3, {4, 5, 6}};
 * assert(static_b.data[0] == 4);
 * assert(static_b.data[1] == 5);
 * assert(static_b.data[2] == 6);
 * static array_int_t static_c = {3, {[0 ... 2]=7}};
 *
 * auto_free array_int_t* heap_a = smart_array_heap_new(int, 100, malloc);
 * d = heap_a->data;
 * for (int i = 0; i < heap_a->len; ++i) {d[i]=i;}
 * for (int i = 0; i < heap_a->len; ++i) {assert(d[i] == i);}
 * ```
 */
#define smart_array(T) \
    struct smart_array_##T { \
        unsigned int len; \
        T data[]; \
    }


/* Allocate smart_array on stack.
 *
 * Example:
 * ```
 * array_int_t* stack_a = smart_array_stack_new(int, 100);
 * int* d = stack_a->data;
 * for (int i = 0; i < stack_a->len; ++i) {d[i]=i;}
 * for (int i = 0; i < stack_a->len; ++i) {assert(d[i] == i);}
 * ```
 */
#define smart_array_stack_new(T, alen) \
    ({ \
    typeof (alen) _ARRAY_len = (alen); \
    struct smart_array_##T* ptr = (struct smart_array_##T*) \
        __builtin_alloca(sizeof(struct smart_array_##T) + _ARRAY_len*sizeof(T)); \
    ptr->len = _ARRAY_len; \
    ptr;})

/* Allocate smart_array on heap.
 *
 * Example:
 * ```
 * auto_free array_int_t* heap_a = smart_array_heap_new(int, 100, malloc);
 * ```
 */
#define smart_array_heap_new(T, alen, allocator) \
    ({ \
    typeof (alen) _ARRAY_len = (alen); \
    struct smart_array_##T* ptr = (struct smart_array_##T*) \
        allocator(sizeof(struct smart_array_##T) + _ARRAY_len*sizeof(T)); \
    ptr->len = _ARRAY_len; \
    ptr;})
