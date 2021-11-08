#include "slang_codegen/command_line_args/command_line_args.h"

std::optional<const slang::llvm_code_generator::CommandLineArgs>
slang::llvm_code_generator::CommandLineArgs::Parse(
    const std::vector<std::string_view> &args, std::ostream &outs,
    std::ostream &errs, std::ostream &logs) noexcept {
  CommandLineArgs result;

  if (args.size() < 2) {
    return std::nullopt;
  }

  // TODO: add support for help info flag.

  result.in_path_ = std::filesystem::path(args[1]).lexically_normal();

  (result.out_path_ = result.InputPath()).replace_extension(L".obj");

  return result;
}

const slang::llvm_code_generator::CommandLineArgs::PathType &
slang::llvm_code_generator::CommandLineArgs::InputPath() const noexcept {
  return in_path_;
}

const slang::llvm_code_generator::CommandLineArgs::PathType &
slang::llvm_code_generator::CommandLineArgs::OutputPath() const noexcept {
  return out_path_;
}
