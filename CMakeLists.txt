cmake_minimum_required(VERSION 3.12)

project(SLang-LLVM VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(utils)
add_subdirectory(code_generator)
