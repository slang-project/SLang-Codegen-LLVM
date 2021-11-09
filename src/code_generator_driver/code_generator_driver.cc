#include "slang_codegen/code_generator_driver/code_generator_driver.h"

#include <fstream>
#include <system_error>

#include "ir_converter/ir_converter.h"
#include "object_file_generator/object_file_generator.h"

bool slang::llvm_code_generator::driver::Main(const CommandLineArgs &args,
                                              std::ostream &outs,
                                              std::ostream &errs,
                                              std::ostream &logs) noexcept {
  const auto &in_path = args.InputPath();
  if (std::error_code ec; !std::filesystem::exists(in_path, ec) || ec) {
    // TODO: message on file not found.
    return false;
  }

  // TODO: \c std::basic_ifstream<char8_t> (or \c std::u8ifstream if present).
  std::ifstream in_file(in_path);
  if (!in_file.is_open()) {
    // TODO: message on file open failure.
    return false;
  }

  // TODO: move to \c noexcept version when it will be available in the library.
  const auto parsed_content = nlohmann::json::parse(in_file, nullptr, false);
  in_file.close();
  if (parsed_content.is_discarded()) {
    // TODO: message on parsing failure.
    return false;
  }

  const auto program_module = ir::Convert(parsed_content);
  if (!program_module) {
    // TODO: message on code conversion failure.
    return false;
  }

  if (!object_file_generator::ConvertIrToObjectFile(*program_module,
                                                    args.OutputPath())) {
    // TODO: message on object file generation failure.
    return false;
  }

  return true;
}
