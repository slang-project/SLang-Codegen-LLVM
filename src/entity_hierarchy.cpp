#include "entity_hierarchy.hpp"


std::unique_ptr<ObjectAST> fromJson (const json& input)
{
    std::cout << input["type"] << std::endl;
    return std::unique_ptr<ObjectAST>(new NumberExpressionAST(7.0));
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

Value *NumberExpressionAST::codegen()
{
    return ConstantFP::get(TheContext, APFloat(Val));
}