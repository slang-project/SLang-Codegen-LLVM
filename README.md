# SLang LLVM

## SLang Programming Language 'middle-end' implementation with LLVM Framework.
Convert JSON IR of SLang program to LLVM IR which can be compiled into binary with LLVM Backend.

## How to build and run?
>assuming current working directory is root of project, i.e. SLang-LLVM

### Linux (Ubuntu)
#### Prerequisites
`LLVM Tools`
>refer to instructions on [llvm repository](https://apt.llvm.org/)

`G++ (optionally disabled, i.e. if you use clang++)`
```bash
sudo apt install g++
# change compiler config in CMakeFiles.txt to use clang
```
`CMake`
```bash
sudo apt install cmake
```
`Ninja build system`
```bash
sudo apt install ninja-build
```
#### Compile

`Manual:`
```bash
mkdir build && cd build
cmake ../
ninja
```
`VS Code:`
>Use **(gdb) Launch** configuration for starting with default input\
>Use Build task **ninja** (Ctrl+Shift+B) for building only\
>Use Clean task **clean_build_executable** for deleting "build" and "executable" directories 
#### Run
Go to project root.

Use tests
```bash
./executable/slang_jtll ./tests/in.json
```
