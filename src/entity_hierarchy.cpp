#include "entity_hierarchy.hpp"


std::unique_ptr<ObjectAST> fromJson (const json& input)
{
    std::cout << input["type"] << std::endl;
    return std::unique_ptr<ObjectAST>(new IntegerExpressionAST(7.0));
}

static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::map<std::string, Value *> NamedValues;


std::unique_ptr<ExpressionAST> LogError(const char *Str) {
  fprintf(stderr, "Error: %s\n", Str);
  return nullptr;
}

Value *LogErrorV(const char *Str) {
  LogError(Str);
  return nullptr;
}

Value *IntegerExpressionAST::codegen()
{
  return ConstantInt::get(TheContext, APInt(32, Val, true));
}

Value *BinaryExpressionAST::codegen()
{
  Value *L = LHS->codegen();
  Value *R = RHS->codegen();
  if (!L || !R)
    return nullptr;

  switch (Op) {
  case '+':
    return Builder.CreateAdd(L, R, "addtmp");
  case '-':
    return Builder.CreateSub(L, R, "subtmp");
  case '*':
    return Builder.CreateMul(L, R, "multmp");
  case '<':
    return Builder.CreateICmpULT(L, R, "cmptmp");
  default:
    return LogErrorV("invalid binary operator");
  }
}