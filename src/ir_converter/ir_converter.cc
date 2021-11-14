#include "ir_converter.h"

#define JSON_NOEXCEPTION
#include "nlohmann/json-schema.hpp"
#undef JSON_NOEXCEPTION

std::unique_ptr<const slang::llvm_code_generator::ir::LlvmIr>
slang::llvm_code_generator::ir::Convert(const SlangIr & /*program*/) noexcept {
  /// TODO(deiuch): implement converter.
  return nullptr;
}
