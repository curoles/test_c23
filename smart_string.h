#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "c23defines.h"

typedef struct smart_string
{
    size_t capacity;
    size_t len;
    char* storage;
} smart_string_t;

constexpr size_t smart_string_max_len = 1024* 1024;

static inline
__attribute__((nonnull(1)))
void
smart_string_init(smart_string_t *restrict self, size_t len)
{
    len += 1; // to have space for '\0'

    self->storage = (char*) malloc(len*sizeof(char));
    assert(self->storage != nullptr);

    self->storage[0] = '\0';
    self->len = 0;
    self->capacity = len;
}

static inline
__attribute__((nonnull(1)))
void
smart_string_free(smart_string_t *restrict self)
{
    free(self->storage);
    self->storage = nullptr;
}

static inline
void
smart_string_cleanup(smart_string_t* self)
{
    smart_string_free(self);
}

#define SMART_STRING(name, len) \
    __attribute__((cleanup(smart_string_cleanup))) \
    smart_string_t name; \
    smart_string_init(&name, len);

#define SMART_STRING_FROM(name, str) \
    __attribute__((cleanup(smart_string_cleanup))) \
    smart_string_t name; \
    smart_string_from(&name, (str));

static inline
FN_ATTR_RETURNS_NONNULL __attribute__((nonnull(1)))
smart_string_t*
smart_string_from(smart_string_t *restrict self, const char *restrict s)
{
    size_t len = strnlen(s, smart_string_max_len);
    smart_string_init(self, len);

    strncpy(self->storage, s, self->capacity);
    self->len = self->capacity - 1;

    return self;
}

static inline
FN_ATTR_RETURNS_NONNULL __attribute__((nonnull(1)))
const char*
smart_string_get(smart_string_t* self)
{
    return self->storage;
}

static inline
FN_ATTR_RETURNS_NONNULL __attribute__((nonnull(1)))
smart_string_t*
smart_string_resize(smart_string_t* self, size_t new_len)
{
    new_len += 1;
    if (new_len > self->capacity)
    {
        self->storage = (char*) realloc(self->storage, new_len*sizeof(char));
        self->capacity = new_len;
    }

    return self;
}

static inline
FN_ATTR_RETURNS_NONNULL __attribute__((nonnull(1)))
smart_string_t*
smart_string_append(smart_string_t* self, const char *restrict str)
{
    size_t add_len = strnlen(str, smart_string_max_len);
    size_t total_len = self->len + add_len;

    if (total_len > (self->capacity - 1))
    {
        smart_string_resize(self, total_len);
    }

    strncpy(&self->storage[self->len], str, add_len+1);

    return self;
}
