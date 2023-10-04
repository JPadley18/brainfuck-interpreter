#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include "optimize.h"

/**
 * Compresses a brainfuck string by removing non-brainfuck chars
 * 
 * @param source char pointer to the null-terminated code source
 * @param dest char pointer to a destination buffer
*/
void compress(char *source, char* dest) {
    regex_t regex;
    int err = regcomp(&regex, "[\\[\\]\\+\\-\\.\\,\\<\\>]", 0);
    if(err) {
        printf("Error compiling compression regex\n");
        return;
    }

    char c;
    int j = 0;
    for(int i = 0; (c = source[i]) != '\0'; i++) {
        char inputStr[2] = {c, '\0'};
        if(!regexec(&regex, inputStr, 0, NULL, 0)) {
            dest[j++] = c;
        }
    }
    dest[j] = '\0';

    regfree(&regex);
}