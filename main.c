#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char* argv[]) {
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
    return 0;
}