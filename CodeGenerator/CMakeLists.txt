cmake_minimum_required(VERSION 3.12)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(CodeGenerator "main.cpp")

# llvm_map_components_to_libnames(llvm_libs support core irreader)

# target_link_libraries(CodeGenerator ${llvm_libs})
