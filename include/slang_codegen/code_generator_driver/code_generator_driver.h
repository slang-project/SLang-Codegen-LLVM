/**
 * \file
 * Entry point of the SLang compiler LLVM code generator.
 * \author Denis Chernikov (@deiuch)
 * \date 2021
 */
#ifndef SLANG_CODEGEN_LLVM_CODE_GENERATOR_DRIVER_HPP
#define SLANG_CODEGEN_LLVM_CODE_GENERATOR_DRIVER_HPP

#include <iostream>

#include "slang_codegen/command_line_args/command_line_args.h"

namespace slang::llvm_code_generator::driver {
/**
 * \brief Execute code generator.
 * \param[in]  args Command line arguments.
 * \param[out] outs Stream where program's output will go.
 * \param[out] errs Stream where program's error will go.
 * \param[out] logs Stream where program's log will go.
 * \return \c true - program has finished successfully; \c false - otherwise.
 */
[[nodiscard]] bool Main(const CommandLineArgs &args,
                        std::ostream &outs = std::cout,
                        std::ostream &errs = std::cerr,
                        std::ostream &logs = std::clog) noexcept;
}  // namespace slang::llvm_code_generator::driver

#endif
