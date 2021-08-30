# cpp-brainf-interpreter
Yet another unoriginal brainf*** interpreter written in c++.  
This project is similar to many already existing implementations of bf interpreters, so I won't explain the language entirely (learn more [here](https://en.wikipedia.org/wiki/Brainfuck)), however here are some key details:
- The size of the array is 30000
- The call stack size is 1024
- Each cell stores one byte [0, 255]
- Overflow and underflow is well defined

## Usage
Build the interpreter using gnu `make` in the base directory. It will appear in the `bin` folder. Then, run the executable with the relative path of the source file as the first and only command line argument.

```sh
cd path/to/dir
make
./bin/bf tests/helloworld.bf
```