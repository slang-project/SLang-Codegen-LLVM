#include "entity_hierarchy.hpp"

static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::map<std::string, Value *> NamedValues;

//TODO: copy all function declarations to header
void initialize()
{
  TheModule = llvm::make_unique<Module>("test_codegen", TheContext);
}

void print_generated_code()
{
  TheModule->print(errs(), nullptr);
}

Value *LogErrorV(const char *Str)
{
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

Value *IntegerExpressionAST::codegen()
{
  return ConstantInt::get(TheContext, APInt(32, Val, true));
}

Value *VariableExpressionAST::codegen() {}

Value *BinaryExpressionAST::codegen()
{
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R)
    return nullptr;

  switch (Op)
  {
  case BinaryOp::add:
    return Builder.CreateAdd(L, R, "addtmp");
  case BinaryOp::subtract:
    return Builder.CreateSub(L, R, "subtmp");
  case BinaryOp::multiply:
    return Builder.CreateMul(L, R, "multmp");
  case BinaryOp::less_than:
    return Builder.CreateICmpULT(L, R, "cmptmp");
  default:
    return LogErrorV("invalid binary operator");
  }
}
