cmake_minimum_required(VERSION 3.16)

project(
    SLang-Codegen-LLVM
    VERSION 0.2.1
    DESCRIPTION "SLang compiler backend that uses LLVM to generate target code."
    LANGUAGES C CXX
)

set(SLANG_CODEGEN_SRC_PATH "${PROJECT_SOURCE_DIR}/src")
set(SLANG_CODEGEN_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SLANG_CODEGEN_BUILD_TESTS OFF)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

find_package(nlohmann_json CONFIG REQUIRED)

find_package(LLVM CONFIG REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
add_definitions(${LLVM_DEFINITIONS})

set(SLANG_CODEGEN_ALL_SOURCE_FILES)

add_subdirectory(src)
add_subdirectory(app)

add_custom_target(
    clang-format
    COMMAND clang-format
    --style=file
    --verbose
    -i
    ${SLANG_CODEGEN_ALL_SOURCE_FILES}
)
