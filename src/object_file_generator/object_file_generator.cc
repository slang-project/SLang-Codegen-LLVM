#include "object_file_generator.h"

bool slang::llvm_code_generator::object_file_generator::ConvertIrToObjectFile(
    const llvm::Module & /*module*/,
    const std::filesystem::path & /*out_path*/) noexcept {
  /// TODO(deiuch): generate object file with program.
  return false;
}
