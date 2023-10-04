#pragma once

#define NO_COMPRESSION 0
#define STRIP_ONLY 1
#define COMPRESS 2

void compress(char *source, char *dest, int compression);