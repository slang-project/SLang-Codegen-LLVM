cmake_minimum_required(VERSION 3.12)

project(SLang-LLVM VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Global variables
set(ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Project dependencies

# LLVM
find_package(LLVM 9.0.0 REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# nlohmann-json
find_package(nlohmann_json 3.7.3 REQUIRED CONFIG)

add_subdirectory(ir_converter)
add_subdirectory(object_file_generator)
add_subdirectory(command_line_args)
add_subdirectory(code_generator_driver)
add_subdirectory(code_generator)
