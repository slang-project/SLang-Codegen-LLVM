add_executable(SlangCompilerLlvmCodegenDriver code_generator.cpp)

set_target_properties(SlangCompilerLlvmCodegenDriver PROPERTIES
                      CXX_STANDARD_REQUIRED ON
                      CXX_STANDARD 20
                      CXX_EXTENSIONS OFF)

target_link_libraries(SlangCompilerLlvmCodegenDriver PRIVATE SlangCompilerLlvmCodegen)
target_include_directories(SlangCompilerLlvmCodegenDriver PRIVATE ${SLANG_CODEGEN_INCLUDE_DIR})
