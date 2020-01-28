cmake_minimum_required(VERSION 3.12)

find_package(LLVM 9.0.0 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

find_package(nlohmann_json 3.2.0 REQUIRED CONFIG)

add_executable(code_generator main.cpp code_generator_driver.cpp)

# llvm_map_components_to_libnames(llvm_libs support core irreader)

# target_link_libraries(code_generator ${llvm_libs})
target_link_libraries(code_generator PRIVATE nlohmann_json::nlohmann_json utils)

target_include_directories(code_generator PRIVATE ${ROOT_PATH})
