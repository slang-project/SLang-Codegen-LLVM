/**
 * \file
 * \author Denis Chernikov (@deiuch)
 * \date 2021
 */
#ifndef SLANG_CODEGEN_LLVM_OBJECT_FILE_GENERATOR_H
#define SLANG_CODEGEN_LLVM_OBJECT_FILE_GENERATOR_H

#include <filesystem>

#include "llvm/IR/Module.h"

namespace slang::llvm_code_generator::object_file_generator {
  /**
   * \brief Parse given program tree and compile an object file out of it.
   * \param[in] module LLVM Module to generate object file from.
   * \param[in] out_path Path to the place where an object file should be stored.
   * \return \c true - file was generated successfully; \c false - otherwise.
   * \note If some file exists at \p out_path, it will be overwritten.
   */
  [[nodiscard]] bool ConvertIrToObjectFile(
      const llvm::Module &module,
      const std::filesystem::path &out_path) noexcept;
} // namespace slang::llvm_code_generator::object_file_generator

#endif
