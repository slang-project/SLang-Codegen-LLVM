cmake_minimum_required(VERSION 3.16)

project(
    SLang-Codegen-LLVM
    VERSION 0.2.1
    DESCRIPTION "SLang compiler backend that uses LLVM to generate target code."
    LANGUAGES C CXX
)

set(SLANG_CODEGEN_SRC_PATH "${PROJECT_SOURCE_DIR}/src")
set(SLANG_CODEGEN_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

find_package(LLVM CONFIG REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
add_definitions(${LLVM_DEFINITIONS})

find_package(nlohmann_json_schema_validator REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(SLANG_CODEGEN_ALL_SOURCE_FILES)

add_subdirectory(src)
add_subdirectory(app)

find_program(CLANG_FORMAT clang-format)
if(NOT CLANG_FORMAT)
    message(STATUS "Did not find clang-format, target Format is disabled.")
else()
    add_custom_target(
        Format COMMAND
        ${CLANG_FORMAT}
        --dry-run
        --style=file
        --verbose
        --Werror
        -i
        ${SLANG_CODEGEN_ALL_SOURCE_FILES}
    )
endif()

find_program(CLANG_TIDY clang-tidy)
if(NOT CLANG_TIDY)
    message(STATUS "Did not find clang-tidy, target Tidy is disabled.")
else()
    add_custom_target(
        Tidy COMMAND
        ${CLANG_TIDY}
        -p ${CMAKE_BINARY_DIR}/compile_commands.json
        ${SLANG_CODEGEN_ALL_SOURCE_FILES}
    )
endif()
