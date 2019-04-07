# Project structure
1. apps binaries located here
2. include library headers
  * ASMTranslatorForStackMachine - Translates AST into stack-machine assembly after which produces machine-code.
  * ASMInterpreter - Interprets produced machine-code.
  * Calculator - Defines classes for tokenization, AST building and evaluation of arithmetical expressions.
3. src - source codes of mentioned libraries

# TODO
1. Add capabilty to produces machine-code both for stack-based and register-based machines.
2. Add PLI support to inspect machines internal work through waveforms.
3. Add negative numbers support.
4. Add float point numbers support.
5. Add separate data clases.
6. Main refactoring.
7. Drink a cup of kaffee.

# How to build
`mkdir build && cd build`

`cmake ..`

`make`

After these steps, you should see compiled binaries for files from apps folder.
