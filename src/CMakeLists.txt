set(SLANG_CODEGEN_SOURCE_LIST)
set(SLANG_CODEGEN_PUBLIC_HEADER_LIST)
set(SLANG_CODEGEN_PRIVATE_HEADER_LIST)
set(SLANG_CODEGEN_TEST_SOURCE_LIST)

add_subdirectory(ir_converter)
add_subdirectory(object_file_generator)
add_subdirectory(command_line_args)
add_subdirectory(code_generator_driver)

set(
    SLANG_CODEGEN_ALL_SOURCE_FILES
    ${SLANG_CODEGEN_ALL_SOURCE_FILES}
    ${SLANG_CODEGEN_SOURCE_LIST}
    ${SLANG_CODEGEN_PUBLIC_HEADER_LIST}
    ${SLANG_CODEGEN_PRIVATE_HEADER_LIST}
    ${SLANG_CODEGEN_TEST_SOURCE_LIST}
    PARENT_SCOPE
)

add_library(
    SlangCompilerLlvmCodegen STATIC
    ${SLANG_CODEGEN_SOURCE_LIST}
    ${SLANG_CODEGEN_PUBLIC_HEADER_LIST}
    ${SLANG_CODEGEN_PRIVATE_HEADER_LIST}
)

set_target_properties(
    SlangCompilerLlvmCodegen PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
)

target_include_directories(
    SlangCompilerLlvmCodegen
    PUBLIC ${SLANG_CODEGEN_INCLUDE_DIR}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

# LLVM
llvm_map_components_to_libnames(
    LLVM_LIBS
    Support
    Core
)
target_include_directories(
    SlangCompilerLlvmCodegen
    PRIVATE ${LLVM_INCLUDE_DIRS}
)
target_link_libraries(
    SlangCompilerLlvmCodegen
    PRIVATE ${LLVM_LIBS}
)

# JSON library by nlohmann
target_link_libraries(
    SlangCompilerLlvmCodegen
    PRIVATE nlohmann_json nlohmann_json::nlohmann_json
)

# JSON Schema validation library by pboettch
target_link_libraries(SlangCompilerLlvmCodegen
    PRIVATE nlohmann_json_schema_validator
)

# Unit testing
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_executable(
        SlangCompilerLlvmCodegenUnitTests
        ${SLANG_CODEGEN_TEST_SOURCE_LIST}
    )
    set_target_properties(
        SlangCompilerLlvmCodegenUnitTests PROPERTIES
        CXX_STANDARD_REQUIRED ON
        CXX_STANDARD 20
        CXX_EXTENSIONS OFF
    )
    target_link_libraries(
        SlangCompilerLlvmCodegenUnitTests
        PRIVATE SlangCompilerLlvmCodegen
        PRIVATE GTest::gtest_main
    )
    target_include_directories(
        SlangCompilerLlvmCodegenUnitTests
        PRIVATE ${SLANG_CODEGEN_INCLUDE_DIR}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    )
    gtest_discover_tests(
        SlangCompilerLlvmCodegenUnitTests
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
endif()
