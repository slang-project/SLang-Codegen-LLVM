#ifndef SLANG_CODEGEN_LLVM_CODE_GENERATOR_DRIVER_HPP
#define SLANG_CODEGEN_LLVM_CODE_GENERATOR_DRIVER_HPP

#include <iostream>

#include "slang_codegen/command_line_args/command_line_args.h"

namespace slang::llvm_code_generator::driver {
/**
 * \brief Execute code generator.
 * \param args Command line arguments.
 * \param outs Stream where program's output will go.
 * \param errs Stream where program's error will go.
 * \param logs Stream where program's log will go.
 * \return \c true - program has finished successfully; \c false - otherwise.
 */
[[nodiscard]] bool Main(const CommandLineArgs &args,
                        std::ostream &outs = std::cout,
                        std::ostream &errs = std::cerr,
                        std::ostream &logs = std::clog) noexcept;
}  // namespace slang::llvm_code_generator::driver

#endif
