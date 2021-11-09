set(SLANG_CODEGEN_SOURCE_LIST
    ${SLANG_CODEGEN_SOURCE_LIST}
    "${CMAKE_CURRENT_SOURCE_DIR}/code_generator_driver.cc"
    PARENT_SCOPE)

set(SLANG_CODEGEN_PUBLIC_HEADER_LIST
    ${SLANG_CODEGEN_PUBLIC_HEADER_LIST}
    "${SLANG_CODEGEN_INCLUDE_DIR}/slang_codegen/code_generator_driver/code_generator_driver.h"
    PARENT_SCOPE)
