# SLang-Codegen-LLVM

## Overview

This project is an implementation of the SLang programming language compiler backend using LLVM.
You may find an old implementation on the [branch `v0.1.3`](../../tree/v0.1.3).

## How to install dependencies and build project

This project is based on CMake. In order to build it, you need to install dependencies and then build the project.

Also consider installation of `clang-format` and `clang-tidy` tools to conform to project's coding standard.
The version used as of the writing of this manual is `13.0.0`.

You can run the corresponding check using CMake targets `Format` and `Tidy`.

### Visual Studio Code

Cross-platform universal solution for development for this project is to use the [Visual Studio Code](https://code.visualstudio.com).

Please, consider installing all recommended extensions for this workspace and take care of the dependencies based on the instructions below.

### UNIX

After cloning the project, run the tool that installs the `vcpkg` and sets a global environment variable with a path to it:
```bash
tools/install_vcpkg.sh
```

By default, it installs `vcpkg` in a folder near project's folder. You may customise this behaviour by providing the install path by your own.
You are strongly advised to not install it in any of the project folder or subfolders.
```bash
tools/install_vcpkg.sh path/to/vcpkg
```

As an alternative, you can install all the required dependencies by yourself. Yse `vcpkg.json` and `Dockerfile` for hitns on what you may need to install.
> This method is not advised since it introduces some inconsistency into project development.

### Windows

You are advised to set this option before cloning this project to not make you editor go mad.
```bat
git config --global core.autocrlf input
```

You also will need to have [`vcpkg`](https://github.com/Microsoft/vcpkg) package manager installed:
```bat
REM Put vcpkg out of project scope
cd ..

REM If fails, remove `/m` option to setup variable just for user
SETX VCPKG_ROOT "%cd%\vcpkg" /m

git clone https://github.com/microsoft/vcpkg %VCPKG_ROOT%
%VCPKG_ROOT%\bootstrap-vcpkg.bat
%VCPKG_ROOT%\vcpkg integrate install
```

#### Visual Studio

Instead of VS Code you may want to work in [Visual Studio](https://visualstudio.microsoft.com/vs), which is also supported.

You are advised to use 2019 edition and later with the following individual components installed:
* MSVC v142 (or later)
* C++ CMake tools for Windows

Open this project's folder as a CMake project.

Don't forget to setup command line arguments for debug:
* `.vs\launch.vs.json`
    * `{..."configurations": [...{..."args": "path/to/in.debug.slangir"...}...]...}`
* Using GUI
    * Right-click on top `CMakeLists.txt`
    * "Debug and Launch Settings"

### Docker

The project is packed into reproducible build environment with Docker.
You may use [Dockerfile](Dockerfile) provided to build your own image to work with project.
We are considering publishing the resulting image with dependencies somewhere for quick start on a project.

The following example may be helpful in getting started, but you still need to get [Docker](https://docs.docker.com/engine/install) by yourself ([Ubuntu instruction](https://docs.docker.com/engine/install/ubuntu)):

```bash
docker build --target install-vcpkg --network "host" -t slang-project/slang-codegen-llvm-deps:0.2.1 .
docker build --cache-from slang-project/slang-codegen-llvm-deps:0.2.1 --network "host" -t slang-project/slang-codegen-llvm:0.2.1 .
docker run --rm slang-project/slang-codegen-llvm:0.2.1 path/to/in.json
```

## How to run project

```bash
build/app/SlangCompilerLlvmCodegenDriver path/to/in.slangir
```
