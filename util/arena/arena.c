#include "arena.h"

#include <assert.h>
#include <stdlib.h>

void arena_init(arena_t *arena, void *allocation, size_t size)
{
    assert(arena);
    assert(allocation);
    arena->begin = allocation;
    arena->end = allocation + size;
    arena->ptr = allocation;
}

void arena_clear(arena_t *arena)
{
    if (arena) arena->ptr = arena->begin;
}

void *arena_alloc(arena_t *arena, size_t size)
{
    assert(arena);
    assert(arena->begin);
    assert(arena->ptr);
    assert(arena->end);

    if (arena_available(arena) < size) return NULL;
    void *allocation = arena->ptr;
    arena->ptr += size;
    return allocation;
}
