#include <stdlib.h>
#include "stack.h"

/**
 * Create a stack of given maximum size
 * 
 * @return a pointer to the stack struct created
*/
stack *createStack(int size) {
    stack *newStack = malloc(sizeof(stack));
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
int stackPop(stack *stack) {
    // Return top value and decrement top pointer
    stack->top--;
    return stack->stack[stack->top];
}

/**
 * Push item onto the stack
*/
void stackPush(stack *stack, int item) {
    stack->stack[stack->top++] = item;
}

/**
 * Check if the stack is full
 * 
 * @return true if the stack is full
*/
bool stackIsFull(stack *stack) {
    return stack->top >= stack->size -1;
}

/**
 * Check if the stack is empty
 * 
 * @return true if the stack is empty
*/
bool stackIsEmpty(stack *stack) {
    return stack->top == 0;
}

/** 
 * Free the memory associated with a stack
*/
void freeStack(stack *stack) {
    free(stack->stack);
    free(stack);
}