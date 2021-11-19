#include "ir_converter.h"

#include "gtest/gtest.h"

TEST(IrConverter, NullInput) {
  const auto llvm_context = std::make_unique<llvm::LLVMContext>();
  EXPECT_NE(slang::llvm_code_generator::ir::Convert(nullptr, *llvm_context), nullptr);
}
