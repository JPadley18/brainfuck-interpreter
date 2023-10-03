#pragma once

#include <stdbool.h>

typedef struct stack {
    int top;
    int size;
    int *stack;
} stack;

stack *createStack(int size);
int stackPop(stack *stack);
void stackPush(stack *stack, int item);
bool stackIsFull(stack *stack);
bool stackIsEmpty(stack *stack);
void freeStack(stack *stack);