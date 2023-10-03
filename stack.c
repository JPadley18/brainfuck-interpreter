#include <stdlib.h>
#include "stack.h"

/**
 * Create a stack of given maximum size
 * 
 * @return a pointer to the stack struct created
*/
Stack *create_stack(int size) {
    Stack *newStack = malloc(sizeof(Stack));
    newStack->top = 0;
    newStack->size = size;
    newStack->stack = malloc(size * sizeof(int));
    return newStack;
}

/**
 * Remove and return the top item of the stack
 * 
 * @return the top item of the stack
*/
int stack_pop(Stack *stack) {
    // Return top value and decrement top pointer
    stack->top--;
    return stack->stack[stack->top];
}

/**
 * Push item onto the stack
*/
void stack_push(Stack *stack, int item) {
    stack->stack[stack->top++] = item;
}

/**
 * Check if the stack is full
 * 
 * @return true if the stack is full
*/
bool stack_is_full(Stack *stack) {
    return stack->top >= stack->size -1;
}

/**
 * Check if the stack is empty
 * 
 * @return true if the stack is empty
*/
bool stack_is_empty(Stack *stack) {
    return stack->top == 0;
}