# SLang-Codegen-LLVM

## Overview

This project is an implementation of the SLang programming language compiler backend using LLVM.
You may find an old implementation on the [branch `v0.1.3`](../../tree/v0.1.3).

## How to install dependencies and build project

This project is based on CMake. In order to build it, you need to install dependencies and then build the project.

### Docker

The project is packed into reproducible build environment with Docker.
You may use [Dockerfile](Dockerfile) provided to build your own image to work with project.
We are considering publishing the resulting image with dependencies somewhere for quick start on a project.

In order for the project to be trully cross-platform, we use [`vcpkg`](https://github.com/Microsoft/vcpkg) as package manager.

The following example may be helpful in getting started (Ubuntu), but you still need to get [Docker](https://docs.docker.com/engine/install/ubuntu/) by yourself:

```bash
sudo docker build --target install-vcpkg --network "host" -t slang-project/slang-codegen-llvm-deps:0.2.1 .
sudo docker build --cache-from slang-project/slang-codegen-llvm-deps:0.2.1 --network "host" -t slang-project/slang-codegen-llvm:0.2.1 .
sudo docker run --rm slang-project/slang-codegen-llvm:0.2.1 path/to/in.json
```

### Windows (non-docker)

* [Visual Studio 2019](https://visualstudio.microsoft.com/vs)
    * Install "Individual components" (usually added with workload "Desktop development with C++")
        * MSVC v142
        * C++ CMake tools for Windows
* [Vcpkg](https://github.com/Microsoft/vcpkg)
    * Setup vcpkg
        * `cd ..  # put vcpkg out of project scope`
        * `git clone https://github.com/microsoft/vcpkg`
        * `.\vcpkg\bootstrap-vcpkg.bat`
        * `.\vcpkg\vcpkg integrate install`
        * Replace `[vcpkg root]` in [CMakeSettings.json](CMakeSettings.json) with path to the vcpkg root
    * Install dependencies
        * `cd SLang-Codegen-LLVM  # do this in the project root`
        * `vcpkg install`
        * This operation might take a while...
* Open project folder from Visual Studio 2019
    * You may just run it with chosen configuration
    * Don't forget to setup command line arguments for debug
        * `.vs\launch.vs.json`
            * `{..."configurations": [...{..."args": "path/to/in.debug.slangir"...}...]...}`
        * Using GUI
            * Right-click on top `CMakeLists.txt`
            * "Debug and Launch Settings"

### MacOS (non-docker, non-vcpkg)

* [Homebrew](https://brew.sh/#install)
    * `brew install cmake`
    * `brew tap nlohmann/json`
    * `brew install nlohmann-json`
    * `brew install llvm@12`
* Build project
    * By hand
        * `cmake -S . -B build && cmake --build build`
        * _**TODO**: check if LLVM is recognized properly_
    * Open using CLion or other IDE that supports CMake appropriately

## How to run project

`SLangCompilerLlvmCodegenDriver path/to/in.slangir`
