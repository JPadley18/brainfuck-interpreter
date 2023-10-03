#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

#define NUM_CELLS 30000
#define STACK_SIZE 1024

#define ERROR_UNDERFLOW -1
#define SUCCESS 0
#define ERROR_OVERFLOW 1
#define ERROR_STACK_OVERFLOW 2
#define ERROR_UNCLOSED_LOOP 3
#define ERROR_UNMATCHED_LOOP_CLOSE 4

// Address of the beginning of the allocated memory space
int *startptr;
// Address of the Brainfuck memory pointer
int memptr = 0;
// Program counter
int pc = 0;
// Stack for storing loop addresses
stack *loopStack;

/**
 * Get the size in bytes of the given opened file
 * 
 * @return the size of the file in bytes
*/
long getFileSize(FILE *fptr) {
    // Seek to the end of the file
    fseek(fptr, 0, SEEK_END);
    // Use current pointer position as file size
    long size = ftell(fptr);
    rewind(fptr);
    return size;
}

/**
 * Reads the given file into memory
 * 
 * @return a pointer to the beginning of the data, NULL if the file cannot be read.
*/
char* readFile(char* filename) {
    FILE *fptr = fopen(filename, "r");
    // Return null if the file cant be opened
    if(fptr == NULL) {
        return NULL;
    }
    // Allocate a buffer to store the file contents + a null char at the end
    long size = getFileSize(fptr);
    char *buffer = malloc(size + 1);
    fread(buffer, size, 1, fptr);
    buffer[size] = 0;

    return buffer;
}

/**
 * Initialise the Brainfuck interpreter values
*/
void initBrainfuck() {
    // Calculate the size of memory needed for allocation
    int size = NUM_CELLS * sizeof(int);
    startptr = malloc(size);
    // Allocate loop stack
    loopStack = createStack(STACK_SIZE);
}

/**
 * Move the brainfuck memory pointer by the specified amount
 * 
 * @return SUCCESS if operation succeeds, ERROR_UNDERFLOW or ERROR_OVERFLOW if not.
*/
int movePointer(int offset) {
    // Bounds checking
    if(memptr + offset < 0) {
        return ERROR_UNDERFLOW;
    } else if(memptr + offset > NUM_CELLS - 1) {
        return ERROR_OVERFLOW;
    }
    // Move the pointer if bounds checking succeeds
    memptr += offset;
    return SUCCESS;
}

/**
 * Runs the given brainfuck code
 * 
 * @return SUCCESS if no errors occur, an error code if an error occurs.
*/
int runBrainfuck(char *code) {
    // True when passing over a loop
    bool skip = false;
    char c;
    // Loop over code until terminator is found
    while((c = code[pc++]) != '\0') {
        int err = SUCCESS;
        if(skip && c != ']'){
            continue;
        }
        switch(c) {
            case '+':
                startptr[memptr]++;
                break;
            case '-':
                startptr[memptr]--;
                break;
            case '<': ;
                err = movePointer(-1);
                break;
            case '>': ;
                err = movePointer(1);
                break;
            case '.':
                putchar(startptr[memptr]);
                break;
            case ',':
                startptr[memptr] = getchar();
                break;
            case '[':
                if(startptr[memptr] == 0) {
                    skip = true;
                } else {
                    // Begin loop
                    if(stackIsFull(loopStack)) {
                        return ERROR_STACK_OVERFLOW;
                    }
                    stackPush(loopStack, pc);
                }
                break;
            case ']':
                if(skip) {
                    skip = false;
                } else if(startptr[memptr] == 0) {
                    stackPop(loopStack);
                    continue;
                } else {
                    // Return to beginning of loop
                    if(stackIsEmpty(loopStack)) {
                        return ERROR_UNMATCHED_LOOP_CLOSE;
                    }
                    pc = stackPop(loopStack) - 1;
                }
                break;
        }

        if(err != SUCCESS) {
            return err;
        }
    }

    // If the loop stack is not empty, a loop was not closed
    if(!stackIsEmpty(loopStack) || skip) {
        return ERROR_UNCLOSED_LOOP;
    }

    return SUCCESS;
}

int main(int argc, char* argv[]) {
    // Initialise the brainfuck memory
    initBrainfuck();

    // Read the filename of the brainfuck file
    if(argc != 2) {
        printf("Expected 1 argument, got %d\n", argc-1);
        return -1;
    }
    char* filename = argv[1];
    // Attempt to open the input file
    char* code = readFile(filename);
    if(code == NULL) {
        printf("Failed to read file \"%s\"\n", filename);
        return -1;
    }

    // Run the brainfuck code
    int err = runBrainfuck(code);

    printf("\n");

    // Print error if it occurred
    switch(err) {
        case ERROR_UNDERFLOW:
            printf("Memory Underflow Error on char %d\n", pc);
            break;
        case ERROR_OVERFLOW:
            printf("Memory Overflow Error on char %d\n", pc);
            break;
        case ERROR_STACK_OVERFLOW:
            printf("Exceeded maximum allowed nested loop level (%d) on char %d\n", STACK_SIZE, pc);
            break;
        case ERROR_UNCLOSED_LOOP:
            printf("Unclosed loop at char %d\n", pc);
            break;
        case ERROR_UNMATCHED_LOOP_CLOSE:
            printf("Unmatched ']' at char %d\n", pc);
            break;
    }

    // Free used memory
    free(code);
    free(startptr);
    freeStack(loopStack);

    if(err == SUCCESS) {
        return 0;
    }
    return -1;
}