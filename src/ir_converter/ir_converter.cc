#include "ir_converter.h"

#include <cassert>
#include <filesystem>
#include <fstream>

#define JSON_NOEXCEPTION
#include "nlohmann/json-schema.hpp"
#undef JSON_NOEXCEPTION

namespace {
  nlohmann::json_schema::json_validator GetSchemaValidator(std::istream &schema_input) {
    auto schema_document = nlohmann::json::parse(schema_input, nullptr, false);
    assert(!schema_document.is_discarded());
    return {std::move(schema_document)};
  }

  nlohmann::json_schema::json_validator GetSchemaValidator(const std::filesystem::path &schema_file_path) {
    std::ifstream schema_input{schema_file_path};
    assert(schema_input.good());
    return GetSchemaValidator(schema_input);
  }

  using SlangIrSchema = nlohmann::json;
  using SlangIrSchemaValidator = nlohmann::json_schema::json_validator;

  SlangIrSchemaValidator GetSlangIrSchemaValidator() {
    // TODO(deiuch): build the schema inside code generator
    return GetSchemaValidator(std::filesystem::current_path() / "data" / "slangir.schema.json");
  }

  bool ValidateSlangIr(const slang::llvm_code_generator::ir::SlangIr &program,
                       const SlangIrSchemaValidator &validator = GetSlangIrSchemaValidator()) {
    nlohmann::json_schema::basic_error_handler error_handler;
    validator.validate(program, error_handler);
    return !error_handler;
  }
} // namespace

std::unique_ptr<const slang::llvm_code_generator::ir::LlvmIr>
slang::llvm_code_generator::ir::Convert(const SlangIr &program, llvm::LLVMContext &llvm_context) {
  if (!ValidateSlangIr(program)) {
    return nullptr;
  }

  /// TODO(deiuch): implement converter
  return std::make_unique<llvm::Module>("TODO", llvm_context);
}
