#pragma once

#include <stdbool.h>

typedef struct Stack {
    int top;
    int size;
    int *stack;
} Stack;

Stack *create_stack(int size);
int stack_pop(Stack *stack);
void stack_push(Stack *stack, int item);
bool stack_is_full(Stack *stack);
bool stack_is_empty(Stack *stack);