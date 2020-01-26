# SLang-LLVM

## Overview

This project is an implementation of the SLang programming language backend using LLVM.
You may find prerequisite development artifacts on branch `v0.1.3`.

## Install and build

This project is based on CMake. In order to build it, you need to have this things done:
* vcpkg
    * https://github.com/Microsoft/vcpkg
	* Install in it:
	    * `nlohmann-json`
* LLVM 9.0.0 built and available for CMake
    * http://releases.llvm.org/download.html#9.0.0

Next run CMake pointing to vcpkg's CMake configs (see `installing and using packages` in vcpkg documentation).
For example, add it onto your CMake command line:
`-DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake`
