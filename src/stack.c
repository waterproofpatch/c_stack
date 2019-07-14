#include <stdio.h>
#include "stack.h"

sstack_t* stack_init(int depth,
                     void* (*allocator)(size_t),
                     void (*deallocator)(void*))
{
    sstack_t* stack = allocator(sizeof(sstack_t));
    if (!stack)
    {
        return NULL;
    }
    stack->arr = allocator(sizeof(char*) * depth);
    if (!stack->arr)
    {
        deallocator(stack);
        return NULL;
    }
    stack->allocator   = allocator;
    stack->deallocator = deallocator;
    stack->depth       = depth;
    stack->top         = -1;   // indicates empty stack
    return stack;
}

void* stack_top(sstack_t* stack)
{
    if (stack->top < 0)
    {
        return NULL;
    }
    return stack->arr[stack->top];
}

void* stack_pop(sstack_t* stack)
{
    if (stack->top < 0)
    {
        return NULL;
    }
    return stack->arr[stack->top--];
}

int stack_push(sstack_t* stack, void* data)
{
    if (stack->top + 1 == stack->depth)
    {
        return 0;
    }
    stack->arr[++stack->top] = data;
    return 1;
}

int stack_destroy(sstack_t* stack)
{
    stack->deallocator(stack->arr);
    stack->deallocator(stack);
    return 1;
}
