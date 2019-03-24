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

Type *LogErrorT(const char *Str)
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

Value *VariableDefinitionAST::codegen()
{
  std::cout << "Parsing Variable Deifinition\n";

  Type *T = type->codegen();
  if (!T)
    return nullptr;

  std::cout << "Memory Allocation\n";

  AllocaInst *v = Builder.CreateAlloca(T, 0, Name);

  if (Body)
  {
    std::cout << "Initialization of Variable\n";

    Value *init = Body->codegen();

    if (!init)
      return nullptr;

    Builder.CreateStore(init, v);
  }

  return v;
}

Type *TypeAST::codegen()
{
  if (name == "Integer")
    return Type::getInt32Ty(TheContext);
  if (name == "Real")
    return Type::getDoubleTy(TheContext);
  if (name == "Void")
    return Type::getVoidTy(TheContext);
  return LogErrorT("invalid type");
}

Function *RoutineDefinitionAST::codegen()
{
  std::cout << "Parsing Routine Arguments\n";

  // vector for routine arguments
  std::vector<Type*> args_type{};

  for (int i = 0; i < Args.size(); i++)
  {
    Type *arg = Args[i]->getType()->codegen();

    if (!arg)
      return nullptr;
    
    args_type.push_back(arg);

    std::cout << "Argument type: " << Args[i]->getName() << "\n";
  }

  std::cout << "Parsing Function Type\n";

  // function type
  Type *functionType = type->codegen();
  
  std::cout << "Create Function\n";
  
  FunctionType *FT =
    FunctionType::get(functionType, args_type, false);

  Function *F =
    Function::Create(FT, Function::ExternalLinkage, Name, TheModule.get());

  // TODO: set names for arguments
  
  std::cout << "Create Basic Block\n";

  BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
  Builder.SetInsertPoint(BB);
  
  std::cout << "Generate Arguments\n";

  for (int i = 0; i < Args.size(); i++) {
    VariableDefinitionAST *var = Args[i];

    AllocaInst *v = Builder.CreateAlloca(args_type[i], 0, var->getName()); 
  }

  std::cout << "Creating function body\n";

  Value *B = Body->codegen();
  if (B) {
    Builder.CreateRet(B);

    verifyFunction(*F);
    return F;
  }

  Builder.CreateRetVoid();
  verifyFunction(*F);
  return F;
}

