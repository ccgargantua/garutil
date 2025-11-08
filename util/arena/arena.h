#ifndef GUTIL_ARENA_H
#define GUTIL_ARENA_H

#include <stddef.h>

typedef struct
{
    void *begin;
    void *ptr;
    void *end;
} arena_t;

void arena_init(arena_t *arena, void *allocation, size_t size);
void arena_clear(arena_t *arena);
void *arena_alloc(arena_t *arena, size_t size);

static inline ptrdiff_t arena_size(const arena_t *arena)
{
    return arena->end - arena->begin;
}

static inline ptrdiff_t arena_available(const arena_t *arena)
{
    return arena->end - arena->ptr;
}

static inline ptrdiff_t arena_occupied(const arena_t *arena)
{
    return arena->ptr - arena->begin;
}

#endif //GUTIL_ARENA_H
