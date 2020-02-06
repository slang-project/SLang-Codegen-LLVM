cmake_minimum_required(VERSION 3.12)

add_executable(code_generator main.cpp)

target_link_libraries(code_generator PRIVATE code_generator_driver)
target_include_directories(code_generator PRIVATE ${ROOT_PATH})
