add_executable(SLangCompilerLlvmCodegenDriver code_generator.cpp)

set_target_properties(SLangCompilerLlvmCodegenDriver PROPERTIES
                      CXX_STANDARD_REQUIRED ON
                      CXX_STANDARD 20
                      CXX_EXTENSIONS OFF)

target_link_libraries(SLangCompilerLlvmCodegenDriver PRIVATE SLangCompilerLlvmCodegen)
target_include_directories(SLangCompilerLlvmCodegenDriver PRIVATE ${SLANG_CODEGEN_INCLUDE_DIR})
