Lab 2.3 - Cross-referencer
==========================

Write a cross-referencer in C that prints a list of all words in a document, and, for each word, a list of the line numbers on which it occurs. Remove noise words like `the`, `and`, and so on.

```
./cross-ref document.txt
```

General Requirements and Considerations
---------------------------------------
- Use the `cross-ref.c` file for your implementation.
- Update `README.md` with the proper steps for building and running your code.
- Follow the command-line arguments convention.
- Don't forget to handle errors properly.
- Coding best practices implementation will be also considered.

Build
-----

To build the code you only need to run

```
make build
```

Or execute directly the compiling and linking gcc comands:

```
gcc -c dictionary.c
gcc -c cross-ref.c
gcc dictionary.o cross-ref.o -o cross-ref
```

Run
---


To build the code you only need to run (this will also build the code):

make run

Or execute directly the compiled file:

./cross-ref


Test cases
----------
Your program will be tested with the following txt-based books.

- [irving-little-573.txt](./irving-little-573.txt)
- [irving-london-598.txt](./irving-london-598.txt)