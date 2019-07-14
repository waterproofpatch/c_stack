#include <stddef.h>
#include <stdlib.h>

#include "unity.h"

// mocks
#include "mock_std_lib_wrapper.h"

// code under test
#include "stack.h"

int g_allocs = 0;
int g_frees  = 0;

void setUp()
{
    g_allocs = 0;
    g_frees  = 0;
}

void tearDown()
{
    TEST_ASSERT_EQUAL(0, g_allocs - g_frees);
}

void* allocator_stub(size_t size, int num_calls)
{
    g_allocs++;
    return malloc(size);
}

void deallocator_stub(void* ptr, int num_calls)
{
    g_frees++;
    free(ptr);
    return;
}

void testStackInitFailSecondAllocator()
{
    void* alloc_1 = malloc(sizeof(sstack_t));
    wrap_malloc_ExpectAndReturn(sizeof(sstack_t), alloc_1);
    wrap_malloc_ExpectAndReturn(sizeof(char*) * 100, NULL);
    wrap_free_Expect(alloc_1);
    sstack_t* stack = stack_init(100, wrap_malloc, wrap_free);
    TEST_ASSERT_NULL(stack);
    free(alloc_1);
}

void testStackInitFailFirstAllocator()
{
    wrap_malloc_ExpectAndReturn(sizeof(sstack_t), NULL);
    sstack_t* stack = stack_init(100, wrap_malloc, wrap_free);
    TEST_ASSERT_NULL(stack);
}

void testStackInit()
{
    wrap_malloc_StubWithCallback(allocator_stub);
    wrap_free_StubWithCallback(deallocator_stub);
    sstack_t* stack = stack_init(100, wrap_malloc, wrap_free);
    TEST_ASSERT_NOT_NULL(stack);
    TEST_ASSERT_EQUAL(wrap_malloc, stack->allocator);
    TEST_ASSERT_EQUAL(wrap_free, stack->deallocator);
    TEST_ASSERT_EQUAL(100, stack->depth);
    TEST_ASSERT_EQUAL(1, stack_destroy(stack));
}

void testStackPushTooManyItems()
{
    wrap_malloc_StubWithCallback(allocator_stub);
    wrap_free_StubWithCallback(deallocator_stub);
    sstack_t* stack = stack_init(100, wrap_malloc, wrap_free);
    TEST_ASSERT_NOT_NULL(stack);
    const int num_items = 100;
    int       items[num_items];
    int       i;
    for (i = 0; i < num_items; i++)
    {
        items[i] = i;
        TEST_ASSERT_EQUAL(1, stack_push(stack, &items[i]));
    }

    int item_wont_fit = 0xdeadbeef;
    TEST_ASSERT_EQUAL(0, stack_push(stack, &item_wont_fit));

    for (i = 99; i >= 0; i--)
    {
        TEST_ASSERT_EQUAL(&items[i], stack_pop(stack));
    }
    TEST_ASSERT_EQUAL(1, stack_destroy(stack));
}

void testStackPushAndPop100Items()
{
    wrap_malloc_StubWithCallback(allocator_stub);
    wrap_free_StubWithCallback(deallocator_stub);
    sstack_t* stack = stack_init(100, wrap_malloc, wrap_free);
    TEST_ASSERT_NOT_NULL(stack);
    const int num_items = 100;
    int       items[num_items];
    int       i;
    for (i = 0; i < num_items; i++)
    {
        items[i] = i;
        TEST_ASSERT_EQUAL(1, stack_push(stack, &items[i]));
    }
    for (i = 99; i >= 0; i--)
    {
        TEST_ASSERT_EQUAL(&items[i], stack_pop(stack));
    }
    TEST_ASSERT_EQUAL(1, stack_destroy(stack));
}

void testStackTopEmpty()
{
    wrap_malloc_StubWithCallback(allocator_stub);
    wrap_free_StubWithCallback(deallocator_stub);
    sstack_t* stack = stack_init(100, wrap_malloc, wrap_free);
    TEST_ASSERT_NOT_NULL(stack);
    TEST_ASSERT_EQUAL(NULL, stack_top(stack));
    TEST_ASSERT_EQUAL(1, stack_destroy(stack));
}
void testStackPopEmpty()
{
    wrap_malloc_StubWithCallback(allocator_stub);
    wrap_free_StubWithCallback(deallocator_stub);
    sstack_t* stack = stack_init(100, wrap_malloc, wrap_free);
    TEST_ASSERT_NOT_NULL(stack);
    TEST_ASSERT_EQUAL(NULL, stack_pop(stack));
    TEST_ASSERT_EQUAL(1, stack_destroy(stack));
}

void testStackPushAndTopOneItem()
{
    wrap_malloc_StubWithCallback(allocator_stub);
    wrap_free_StubWithCallback(deallocator_stub);
    sstack_t* stack = stack_init(100, wrap_malloc, wrap_free);
    TEST_ASSERT_NOT_NULL(stack);
    int item_1 = 1;
    TEST_ASSERT_EQUAL(1, stack_push(stack, &item_1));
    TEST_ASSERT_EQUAL(&item_1, stack_top(stack));
    TEST_ASSERT_EQUAL(&item_1, stack_top(stack));
    TEST_ASSERT_EQUAL(1, stack_destroy(stack));
}

void testStackPushAndPopOneItem()
{
    wrap_malloc_StubWithCallback(allocator_stub);
    wrap_free_StubWithCallback(deallocator_stub);
    sstack_t* stack = stack_init(100, wrap_malloc, wrap_free);
    TEST_ASSERT_NOT_NULL(stack);
    int item_1 = 1;
    TEST_ASSERT_EQUAL(1, stack_push(stack, &item_1));
    TEST_ASSERT_EQUAL(&item_1, stack_pop(stack));
    TEST_ASSERT_EQUAL(1, stack_destroy(stack));
}

void testStackPushOneItem()
{
    wrap_malloc_StubWithCallback(allocator_stub);
    wrap_free_StubWithCallback(deallocator_stub);
    sstack_t* stack = stack_init(100, wrap_malloc, wrap_free);
    TEST_ASSERT_NOT_NULL(stack);
    int item_1 = 1;
    TEST_ASSERT_EQUAL(1, stack_push(stack, &item_1));
    TEST_ASSERT_EQUAL(1, stack_destroy(stack));
}
