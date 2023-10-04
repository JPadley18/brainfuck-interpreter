#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "optimize.h"

/**
 * Compiles a regex with given pattern
 * 
 * @param pattern the regex pattern to compile
 * @param regex a regex_t to compile the regex with
*/
void compile_regex(char *pattern, regex_t *regex) {
    int err = regcomp(regex, pattern, 0);
    if(err) {
        char errBuf[50];
        regerror(err, regex, errBuf, 50);
        printf("Error compiling brainfuck regex with pattern \"%s\": %s\n", pattern, errBuf);
    }
}

/**
 * Removes any chars that do not match the regex from the string
 * 
 * @param source a char pointer to the source string
 * @param dest a char pointer to a destination buffer
 * @param pattern a regex pattern to compare against
*/
void remove_non_matches(char *source, char *dest, char *pattern) {
    regex_t regex;
    compile_regex(pattern, &regex);
    // Run regexec
    int sourceOffset = 0;
    int destOffset = 0;
    regmatch_t matches[regex.re_nsub + 1];
    while(1) {
        int err = regexec(&regex, source + sourceOffset, 1, matches, 0);
        if(err != 0) {
            // Stop regex - out of matches
            break;
        }
        int matchLength = matches[0].rm_eo - matches[0].rm_so;
        memcpy(dest + destOffset, source + sourceOffset + matches[0].rm_so, matchLength);

        sourceOffset += matches[0].rm_eo;
        destOffset += matchLength;
    }

    regfree(&regex);
}

/**
 * Compresses a brainfuck string into its minimal form
 * 
 * @param source char pointer to the null-terminated code source
 * @param dest char pointer to a destination buffer
*/
void compress(char *source, char* dest) {
    // Remove non-brainfuck characters
    remove_non_matches(source, dest, "[][><,.+-]\\{1,\\}");
    //char *temp = malloc(strlen(dest));
    //free(temp);
}