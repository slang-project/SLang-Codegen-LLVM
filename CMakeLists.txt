CMAKE_MINIMUM_REQUIRED(VERSION 3.4.3)


#** Set compiler **
# SET(CMAKE_C_COMPILER "/usr/bin/clang")
# SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")

SET(CMAKE_C_COMPILER "/usr/bin/gcc")
SET(CMAKE_CXX_COMPILER "/usr/bin/g++")

# ^ COMMENT BOTH FOR SYSTEM TO FIND APPROPRIATE COMPILER, E.G. ON WINDOWS

project(SLangLLVM)

#** Version Setup **
SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "1")
SET(VERSION_PATCH "1")

SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

#** Environment Setup **
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../executable)


#** Colored Errors Output **
option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()


#** Discover LLVM **
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})


#** Compilation Setup **
SET(CMAKE_CXX_FLAGS "-g3 -Wall -std=c++11")
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/dependencies/include)
file(GLOB project_sources "${PROJECT_SOURCE_DIR}/src/*.cpp")


#** Compiler Output **
set(EXECUTABLE_NAME slang_jtll)
add_executable(${EXECUTABLE_NAME} ${project_sources})


#** Link LLVM **
llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(${EXECUTABLE_NAME} ${llvm_libs})

# #** Info Tab **
# message("Binary Tree Path ${PROJECT_BINARY_DIR}")
# message("Source Tree Path ${PROJECT_SOURCE_DIR}")
# message("Executables Location ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
# message("Project Version " ${VERSION})

