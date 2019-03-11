# SLang LLVM

## SLang Programming Language 'middle-end' implementation with LLVM Framework.
Convert JSON IR of SLang program to LLVM IR which can be compiled into binary with LLVM Backend.

## How to build and run?
Either with 
```shell
make compile
```
```shell
make run
```
or just
```
make
```
which will do both of first.
Executable will consume first argument as input file path and will try to read it. (`make run` assumes input file as `in.json` from project root)
