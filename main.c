#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "optimize.h"

#define NUM_CELLS 30000
#define CELL_TYPE unsigned char
#define CELL_SIZE (int) sizeof(CELL_TYPE)
#define STACK_SIZE 1024

#define ERROR_UNDERFLOW -1
#define SUCCESS 0
#define ERROR_OVERFLOW 1
#define ERROR_STACK_OVERFLOW 2
#define ERROR_UNCLOSED_LOOP 3
#define ERROR_UNMATCHED_LOOP_CLOSE 4

// Address of the beginning of the allocated memory space
CELL_TYPE *startptr;
// Address of the Brainfuck memory pointer
int memptr = 0;
// Program counter
int pc = 0;
// Stack for storing loop addresses
Stack *loopStack;

/**
 * Get the size in bytes of the given opened file
 * 
 * @return the size of the file in bytes
*/
long get_file_size(FILE *fptr) {
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
char* read_file(char* filename) {
    FILE *fptr = fopen(filename, "r");
    // Return null if the file cant be opened
    if(fptr == NULL) {
        return NULL;
    }
    // Allocate a buffer to store the file contents + a null char at the end
    long size = get_file_size(fptr);
    char *buffer = malloc(size + 1);
    fread(buffer, size, 1, fptr);
    buffer[size] = 0;

    // Compress the brainfuck code
    char *compressBuffer = malloc(size + 1);
    compress(buffer, compressBuffer);
    free(buffer);

    return compressBuffer;
}

/**
 * Initialise the Brainfuck interpreter values
*/
void init_brainfuck() {
    // Calculate the size of memory needed for allocation
    startptr = calloc(NUM_CELLS, CELL_SIZE);
    // Allocate loop stack
    loopStack = create_stack(STACK_SIZE);
}

/**
 * Move the brainfuck memory pointer by the specified amount
 * 
 * @return SUCCESS if operation succeeds, ERROR_UNDERFLOW or ERROR_OVERFLOW if not.
*/
int move_pointer(int offset) {
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
int run_brainfuck(char *code) {
    // > 0 when passing over a loop
    int skip = 0;
    char c;
    // Loop over code until terminator is found
    while((c = code[pc++]) != '\0') {
        int err = SUCCESS;
        if(skip && c != ']' && c != '['){
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
                err = move_pointer(-1);
                break;
            case '>': ;
                err = move_pointer(1);
                break;
            case '.':
                putchar(startptr[memptr]);
                break;
            case ',':
                startptr[memptr] = getchar();
                break;
            case '[':
                if(startptr[memptr] == 0 || skip) {
                    skip += 1;
                } else {
                    // Begin loop
                    if(stack_is_full(loopStack)) {
                        return ERROR_STACK_OVERFLOW;
                    }
                    stack_push(loopStack, pc - 1);
                }
                break;
            case ']':
                if(skip) {
                    skip -= 1;
                } else if(startptr[memptr] == 0) {
                    stack_pop(loopStack);
                } else {
                    // Return to beginning of loop
                    if(stack_is_empty(loopStack)) {
                        return ERROR_UNMATCHED_LOOP_CLOSE;
                    }
                    pc = stack_pop(loopStack);
                }
                break;
        }

        if(err != SUCCESS) {
            return err;
        }
    }

    // If the loop stack is not empty, a loop was not closed
    if(!stack_is_empty(loopStack) || skip) {
        return ERROR_UNCLOSED_LOOP;
    }

    return SUCCESS;
}

/**
 * Get line and char number of program counter index
 * 
 * @param code char pointer to the start of the code string
 * @param out int array[2] to store line num and char num respectively
*/
void get_code_location(char *code, int *out) {
    char c;
    int lineNum = 1;
    int charNum = 1;
    for(int i = 0; i < pc; i++) {
        c = code[i];
        if(c == '\n') {
            lineNum++;
            charNum = 1;
        } else {
            charNum++;
        }
    }
    out[0] = lineNum;
    out[1] = charNum;
}

/**
 * Get a readable trace of the program counter (line # char # (char val))
 * 
 * @param code pointer to the code string
 * @param trace pointer to string buffer to store result
*/
void get_trace(char *code, char *trace) {
    int lineAndChar[2];
    get_code_location(code, lineAndChar);
    char c = code[pc - 1];
    sprintf(trace, "line %d char %d (\"%c\")", lineAndChar[0], lineAndChar[1], c);
}

int main(int argc, char* argv[]) {
    // Initialise the brainfuck memory
    init_brainfuck();

    // Read the filename of the brainfuck file
    if(argc != 2) {
        printf("Expected 1 argument, got %d\n", argc-1);
        return -1;
    }
    char* filename = argv[1];
    // Attempt to open the input file
    char* code = read_file(filename);
    if(code == NULL) {
        printf("Failed to read file \"%s\"\n", filename);
        return -1;
    }

    // Run the brainfuck code
    int err = run_brainfuck(code);

    printf("\n");

    // Print error if it occurred
    char trace[50];
    if(err != SUCCESS) {
        get_trace(code, trace);
    }
    switch(err) {
        case ERROR_UNDERFLOW:
            printf("Memory Underflow Error on %s\n", trace);
            break;
        case ERROR_OVERFLOW:
            printf("Memory Overflow Error on %s\n", trace);
            break;
        case ERROR_STACK_OVERFLOW:
            printf("Exceeded maximum allowed nested loop level (%d) on %s\n", STACK_SIZE, trace);
            break;
        case ERROR_UNCLOSED_LOOP:
            printf("Unclosed loop at %s\n", trace);
            break;
        case ERROR_UNMATCHED_LOOP_CLOSE:
            printf("Unmatched ']' at %sn", trace);
            break;
    }

    // Free used memory
    free(code);
    free(startptr);
    free(loopStack);

    if(err == SUCCESS) {
        return 0;
    }
    return -1;
}