all:
	gcc -Wall -Werror -Wpedantic main.c stack.c optimize.c -o bf -lm