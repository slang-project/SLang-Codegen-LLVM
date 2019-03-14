# SLang LLVM

**SLang Programming Language 'middle-end' implementation with LLVM Framework.**
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
#### Troubleshooting
`-lz linking error`
```bash
sudo apt install zlib1g-dev #for 64-bit should be sufficient
sudo apt install lib32z1-dev #if first not sufficient 
```

### Mac OS

#### Prerequisites
`LLVM Tools`
>install llvm tools, e.g. with:
```shell
brew install llvm
echo 'export PATH="/usr/local/opt/llvm/bin:$PATH"' >> ~/.bash_profile
export LDFLAGS="-L/usr/local/opt/llvm/lib"
export CPPFLAGS="-I/usr/local/opt/llvm/include"
```

`G++ (optionally disabled, i.e. if you use clang++)`
```bash
sudo apt install g++
# change compiler config in CMakeFiles.txt to use clang
```
`CMake`
```bash
brew install cmake
```
`Ninja build system`
```bash
brew install ninja
```
#### Compile

`Manual:`
```bash
mkdir build && cd build
cmake ../
ninja
```
`VS Code:`
>Use **(lldb) Launch** configuration for starting with default input\
>Use Build task **ninja** (⌘+Shift+B) for building only\
>Use Clean task **clean_build_executable** for deleting "build" and "executable" directories 
#### Run
Go to project root.

Use tests
```bash
./executable/slang_jtll ./tests/in.json
```