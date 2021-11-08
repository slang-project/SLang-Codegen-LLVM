#ifndef SLANG_CODEGEN_LLVM_IR_CONVERTER_HPP
#define SLANG_CODEGEN_LLVM_IR_CONVERTER_HPP

#include "llvm/IR/Module.h"

#define JSON_NOEXCEPTION
#include "nlohmann/json.hpp"
#undef JSON_NOEXCEPTION

#include <memory>

namespace slang::llvm_code_generator::ir {
using SlangIr = nlohmann::json;
using LlvmIr = llvm::Module;

/**
 * \brief Convert program represented by SLang IR into LLVM IR Module.
 * \param program SLang IR object with program to convert.
 * \return \c nullptr - conversion failed; LLVM IR Module with program - otherwise.
 */
[[nodiscard]] std::unique_ptr<const LlvmIr> Convert(
    const SlangIr &program) noexcept;
}  // namespace slang::llvm_code_generator::ir

#endif
