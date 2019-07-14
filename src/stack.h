#ifndef _STACK_H
#define _STACK_H

typedef struct _stack_t
{
    void* (*allocator)(size_t);   //! the function to use for allocating
                                  //! resources
    void (*deallocator)(void*);   //! the function to use for freeing resources
    int    top;                   //! index of top of the stack;
    int    depth;                 //! the depth of the stack
    char** arr;                   //! space for the stack
} sstack_t;

/**
 * @brief create a new stack with a fixed depth.
 * @param depth: the size of the stack
 * @param allocator: the allocator function to use for allocating the initial
 * stack and slots.
 * @param deallocator: the deallocator function to use for deallocating memory.
 * @return sstack_t*: the stack to use
 */
sstack_t* stack_init(int depth,
                     void* (*allocator)(size_t),
                     void (*deallocator)(void*));

/**
 * @brief retreive, but do not remove, an item from the stack.
 * @param stack: the stack to get top elem from.
 * @return data: data elem at stack->top, or NULL if no data is on stack.
 */
void* stack_top(sstack_t* stack);

/**
 * @brief remove and retreive an item from the stack. Shrinks the stack by one.
 * @param stack: the stack to pop from.
 * @return data: data elem at stack->top.
 * @return NULL: no data on stack.
 */
void* stack_pop(sstack_t* stack);

/**
 * @brief add an item to the stack.
 * @param stack: the stack to push onto.
 * @param data: the data to push onto the stack. After the push, data will be at
 * stack->top.
 * @return 1: success.
 * @return 0: failure (stack cannot grow any more)
 */
int stack_push(sstack_t* stack, void* data);

/**
 * @brief destroy the stack, freeing all resources allocated on creation. User
 * is responsible for freeing elements pointed by stack.
 * @param stack: the stack to destroy
 * @return 1: success.
 * @return 0: failure.
 */
int stack_destroy(sstack_t* stack);
#endif   // _STACK_H
