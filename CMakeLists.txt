cmake_minimum_required (VERSION 3.12)

project (SLang-LLVM VERSION 0.0 LANGUAGES CXX)

add_subdirectory (SLang-LLVM)

set_property(TARGET SLang-LLVM PROPERTY CXX_STANDARD 17)
