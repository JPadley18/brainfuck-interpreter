#pragma once

struct Stack {
    int top;
    int *stack;
};

Stack* createStack(int size);
int stackPop(Stack* stack);
int stackPush(Stack* stack, int item);