#include "arena/arena.h"
#include "rktest.h"



TEST(arena_tests, init_sanity)
{
    arena_t arena;
    char data[3];
    arena_init(&arena, data, sizeof(data));
    ASSERT_EQ(((char*)arena.begin)[0], data[0]);
    ASSERT_EQ(((char*)arena.begin)[sizeof(data) - 1], data[sizeof(data) - 1]);
    ASSERT_EQ(arena_size(&arena), sizeof(data));
    ASSERT_EQ(arena_available(&arena), sizeof(data));
    ASSERT_EQ(arena_occupied(&arena), 0);
}



TEST(arena_tests, clear_sanity)
{
    arena_t arena;
    char data[256];
    arena_init(&arena, data, sizeof(data));
    ASSERT_EQ(arena.begin, arena.ptr);
    arena_alloc(&arena, sizeof(data) / 2);
    ASSERT_NE(arena.begin, arena.ptr);
    arena_clear(&arena);
    ASSERT_EQ(arena.begin, arena.ptr);
}



TEST(arena_tests, alloc_sanity)
{
    arena_t arena;
    char data[256];
    arena_init(&arena, data, sizeof(data));
    ASSERT_EQ(arena_size(&arena), sizeof(data));
    ASSERT_EQ(arena_available(&arena), sizeof(data));
    ASSERT_EQ(arena_occupied(&arena), 0);
    ASSERT_EQ(arena.begin, arena.ptr);
    arena_alloc(&arena, sizeof(data) / 2);
    ASSERT_EQ(arena.begin + sizeof(data) / 2, arena.ptr);
    ASSERT_EQ(arena.end - sizeof(data) / 2, arena.ptr);
    ASSERT_EQ(arena_size(&arena), sizeof(data));
    ASSERT_EQ(arena_available(&arena), sizeof(data) / 2);
    ASSERT_EQ(arena_occupied(&arena), sizeof(data) / 2);
    arena_alloc(&arena, sizeof(data) / 2);
    ASSERT_EQ(arena.end, arena.ptr);
    ASSERT_EQ(arena_size(&arena), sizeof(data));
    ASSERT_EQ(arena_available(&arena), 0);
    ASSERT_EQ(arena_occupied(&arena), sizeof(data));
    ASSERT_EQ(arena_alloc(&arena, 1), NULL);
}