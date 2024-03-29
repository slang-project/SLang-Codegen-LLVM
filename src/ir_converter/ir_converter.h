/**
 * \file
 * SLang Intermediate Representation and related operations.
 * \author Denis Chernikov (@deiuch)
 * \date 2021
 */
#ifndef SLANG_CODEGEN_LLVM_IR_CONVERTER_H
#define SLANG_CODEGEN_LLVM_IR_CONVERTER_H

#include <memory>

#include "llvm/IR/Module.h"

#define JSON_NOEXCEPTION
#include "nlohmann/json.hpp"
#undef JSON_NOEXCEPTION

namespace slang::llvm_code_generator::ir {
  using SlangIr = nlohmann::json;
  using LlvmIr = llvm::Module;

  /**
   * \brief Convert program represented by SLang IR into LLVM IR Module.
   * \param[in] program SLang IR object with program to convert.
   * \param[in,out] llvm_context LLVM Context object of the code generator.
   * \return \c nullptr - conversion failed; LLVM IR Module with program - otherwise.
   */
  [[nodiscard]] std::unique_ptr<const LlvmIr> Convert(const SlangIr &program, llvm::LLVMContext &llvm_context);
} // namespace slang::llvm_code_generator::ir

#endif
