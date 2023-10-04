# Brainfuck Interpreter
Brainfuck Interpreter written in C

## Optimization
For fun, I decided to add some optimization to this interpreter to speed it up. The optimization process consists of two steps before the code is run.
1. **Stripping** - the brainfuck code is stripped of all non-brainfuck (`[]+-,.<>`) characters to reduce interpreter time spent skipping comment characters.
2. **Compression** - the interpreter compresses the brainfuck code using run-length encoding. All repeated cell operations (`+-<>`) are reduced to one instruction and a `count`, encoded as `[count][instruction]`. E.g. `+++++[<<<<]` becomes `5+[4<]`. The interpreter can then execute these operations in one instruction instead of many smaller steps.

These two steps reduce file size around 2-3x, and increase speed of program execution the same amount.

## Usage
Run 
```
make
```
to create a `bf` executable. With the `bf` executable in the same directory as your brainfuck source code file, run
```
bf your-code.bf
```
to execute the program.