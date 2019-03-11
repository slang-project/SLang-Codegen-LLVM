# SLang LLVM

## SLang Programming Language 'middle-end' implementation with LLVM Framework.
Convert JSON IR of SLang program to LLVM IR which can be compiled into binary with LLVM Backend.

## How to build and run?
>assuming current working directory is root of project, i.e. SLang-LLVM

### Linux (Ubuntu)
#### Prerequisites
`G++`
```bash
sudo apt install g++
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
```bash
mkdir build && cd build
cmake ../
ninja
```
#### Run
If in `build` directory:
```bash
cd ..
```
Use tests
```bash
./executable/slang_jtll ./tests/in.json
```

## How to run?
working directory is root of project, i.e. SLang-LLVM
