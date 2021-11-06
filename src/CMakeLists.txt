set(SLANG_CODEGEN_SOURCE_LIST)
set(SLANG_CODEGEN_PUBLIC_HEADER_LIST)
set(SLANG_CODEGEN_PRIVATE_HEADER_LIST)

add_subdirectory(ir_converter)
add_subdirectory(object_file_generator)
add_subdirectory(command_line_args)
add_subdirectory(code_generator_driver)

add_library(SlangCompilerLlvmCodegen STATIC
            ${SLANG_CODEGEN_SOURCE_LIST}
            ${SLANG_CODEGEN_PUBLIC_HEADER_LIST}
            ${SLANG_CODEGEN_PRIVATE_HEADER_LIST})

set_target_properties(SlangCompilerLlvmCodegen PROPERTIES
                      CXX_STANDARD_REQUIRED ON
                      CXX_STANDARD 20
                      CXX_EXTENSIONS OFF)

target_include_directories(SlangCompilerLlvmCodegen
                           PUBLIC ${SLANG_CODEGEN_INCLUDE_DIR}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# JSON library by nlohmann
target_link_libraries(SlangCompilerLlvmCodegen
                      PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

# LLVM
llvm_map_components_to_libnames(LLVM_LIBS
                                Support
                                Core)
target_include_directories(SlangCompilerLlvmCodegen
                           PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_libraries(SlangCompilerLlvmCodegen
                      PRIVATE ${LLVM_LIBS})
