#ifndef SLANG_CODEGEN_LLVM_COMMAND_LINE_ARGS_HPP
#define SLANG_CODEGEN_LLVM_COMMAND_LINE_ARGS_HPP

#include <filesystem>
#include <iostream>
#include <optional>
#include <string_view>
#include <vector>

namespace slang::llvm_code_generator {
/**
 * \brief Command line arguments container of the code generator.
 */
class CommandLineArgs {
  CommandLineArgs() noexcept = default;

 public:
  using PathType = std::filesystem::path;

  /**
   * \brief Parse provided command line arguments for code generator.
   * \param args Collection of <tt>std::string_view</tt>s on command line argument strings.
   * \param outs Stream where parser's output will go.
   * \param errs Stream where parser's error will go.
   * \param logs Stream where parser's log will go.
   * \return \c std::nullopt - parsing failed; parsed command line arguments' values - otherwise.
   */
  [[nodiscard]] static std::optional<const CommandLineArgs> Parse(
      const std::vector<std::string_view> &args, std::ostream &outs = std::cout,
      std::ostream &errs = std::cerr, std::ostream &logs = std::clog) noexcept;

  /**
   * \brief Get input file path.
   * \return Reference to the input file path object.
   */
  [[nodiscard]] const PathType &InputPath() const noexcept;

  /**
   * \brief Get output file path.
   * \return Reference to the output file path object.
   */
  [[nodiscard]] const PathType &OutputPath() const noexcept;

 private:
  PathType in_path_, out_path_;
};
}  // namespace slang::llvm_code_generator

#endif
