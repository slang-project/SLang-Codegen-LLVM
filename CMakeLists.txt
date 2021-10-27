cmake_minimum_required(VERSION 3.12)

set(CMAKE_TOOLCHAIN_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake
    CACHE STRING "Vcpkg toolchain file")

project(SLangCompiler-LLVM
  VERSION 0.2.0
  DESCRIPTION "SLang compiler backend that uses LLVM to generate target code."
  LANGUAGES C CXX
)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

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

add_subdirectory(src)
add_subdirectory(app)
