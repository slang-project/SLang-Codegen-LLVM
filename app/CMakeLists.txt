set(
    SLANG_CODEGEN_APP_SOURCE_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/code_generator.cc"
)

set(
    SLANG_CODEGEN_ALL_SOURCE_FILES
    ${SLANG_CODEGEN_ALL_SOURCE_FILES}
    ${SLANG_CODEGEN_APP_SOURCE_LIST}
    PARENT_SCOPE
)

add_executable(SlangCompilerLlvmCodegenDriver ${SLANG_CODEGEN_APP_SOURCE_LIST})

set_target_properties(
    SlangCompilerLlvmCodegenDriver PROPERTIES
    CXX_STANDARD_REQUIRED ON
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
)

target_link_libraries(
    SlangCompilerLlvmCodegenDriver
    PRIVATE SlangCompilerLlvmCodegen
)
target_include_directories(
    SlangCompilerLlvmCodegenDriver
    PRIVATE ${SLANG_CODEGEN_INCLUDE_DIR}
)
