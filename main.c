#include <stdio.h>
#include <stdlib.h>

#define NUM_CELLS 1

#define ERROR_UNDERFLOW -1
#define SUCCESS 0
#define ERROR_OVERFLOW 1

// Address of the beginning of the allocated memory space
int *startptr;
// Address of the Brainfuck memory pointer
int memptr = 0;

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
int runBrainfuck(char* code) {
    char c;
    int i = 0;
    // Loop over code until terminator is found
    while((c = code[i++]) != '\0') {
        int err = SUCCESS;
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
        }

        if(err != SUCCESS) {
            return err;
        }
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
            printf("Memory Underflow Error\n");
            break;
        case ERROR_OVERFLOW:
            printf("Memory Overflow Error\n");
            break;
    }

    return 0;
}