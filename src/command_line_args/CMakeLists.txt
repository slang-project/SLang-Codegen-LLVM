set(
    SLANG_CODEGEN_SOURCE_LIST
    ${SLANG_CODEGEN_SOURCE_LIST}
    "${CMAKE_CURRENT_SOURCE_DIR}/command_line_args.cc"
    PARENT_SCOPE
)

set(
    SLANG_CODEGEN_PUBLIC_HEADER_LIST
    ${SLANG_CODEGEN_PUBLIC_HEADER_LIST}
    "${SLANG_CODEGEN_INCLUDE_DIR}/slang_codegen/command_line_args/command_line_args.h"
    PARENT_SCOPE
)
