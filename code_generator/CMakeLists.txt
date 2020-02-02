cmake_minimum_required(VERSION 3.12)

add_executable(code_generator main.cpp code_generator_driver.cpp)

target_link_libraries(code_generator PRIVATE ir_converter command_line_args object_file_generator)
target_include_directories(code_generator PRIVATE ${ROOT_PATH})
