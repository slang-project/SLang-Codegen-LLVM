#include "entity_hierarchy.hpp"


// GLOBAL LLVM CONTAINERS
static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::map<std::string, Value *> NamedValues;


// EXTERNAL INTERFACE
void initLLVMGlobal(std::string moduleName)
{
    TheModule = std::make_unique<Module>(moduleName, TheContext);
}

void printGeneratedCode(std::string outFilePath)
{
    // TODO
}


// HIERARCHY ------------------------------------------------

Function *CompilationAST::codegen()
{
    return LogError<Function>(std::string(__func__) + " not implemented yet");
}

Value *ConditionalIfThenPartAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *ConditionalAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *ThisAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *ReturnExprAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *OldAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *ReferenceAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

// NOTE: SHOULD NOT APPEAR IN FINAL AST! (remove??)
Value *UnresolvedAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *IntegerAST::codegen()
{
    return ConstantInt::get(TheContext, APInt(32, value, true));
}

Value *RealAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *CharacterAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *StringAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *TupleAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

// TODO: think about code generation semantics
Value *MemberAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *CallAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *UnaryAST::codegen()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

/* TODO:
Value *InExprAST::codegen()
{}
*/

Value *BinaryAST::codegen()
{
    //TODO create binary operations versions for all type pairs:
    // e.g. this is special version for (int)a ? (int)b
    Value *L = left->codegen();
    Value *R = right->codegen();
    if (!L || !R)
        return nullptr;

    if(binaryOp == "+")
    {
        return Builder.CreateAdd(L, R, "addtmp");
    }
    else if(binaryOp == "-")
    {
        return Builder.CreateSub(L, R, "subtmp");
    }
    else if(binaryOp == "*")
    {
        return Builder.CreateMul(L, R, "multmp");
    }
    else if(binaryOp == "/")
    {
        return Builder.CreateUDiv(L, R, "divtmp");
    }
    else if(binaryOp == "<")
    {
        return Builder.CreateICmpULT(L, R, "cmptmp");
    }
    else
    {
        return LogError<Value>("Unsupported binary operator.");
    }
}

Type *UnitRefAST::codegen()
{
    if (name == "Integer")
        return Type::getInt32Ty(TheContext);
    if (name == "Real")
        return Type::getDoubleTy(TheContext);
    if (name == "Character")
        return LogError<Type>("Charater not implemented yet.");
    if (name == "String")
        return LogError<Type>("String not implemented yet.");
    return LogError<Type>("Invaid type.");
}

Type *MultiTypeAST::codegen()
{
    return LogError<Type>(std::string(__func__) + " not implemented yet");
}

Type *RangeTypeAST::codegen()
{
    return LogError<Type>(std::string(__func__) + " not implemented yet");
}

/* TODO:
Type *TupleTypeAST::codegen
{};

Type *RoutineTypeAST::codegen
{};
*/

Value *VariableAST::codegen()
{
    Type *T = type->codegen();
    if (!T)
        return nullptr;

    AllocaInst *v = Builder.CreateAlloca(T, 0, name);

    if (initializer)
    {
        Value *init = initializer->codegen();
        if (!init)
            return nullptr;
        Builder.CreateStore(init, v);
    }

    return v;
}

// ? *UnitAST::codegen()
// {
//     return LogError<?>(std::string(__func__) + " not implemented yet");
// }

Function *RoutineAST::codegen()
{
    // vector for routine arguments
    std::vector<Type*> argTypes{};
    std::vector<VariableAST*> argsAsVars{};

    for (auto &arg : parameters)
    {
        VariableAST *curParam = dynamic_cast<VariableAST*>(arg);

        if (!curParam)
            return LogError<Function>("Parameter is not variable declaration.");

        Type *argType = curParam->getType()->codegen();

        if (!argType)
            return nullptr;

        argTypes.push_back(argType);
        argsAsVars.push_back(curParam);
    }

    // function type
    Type *functionType = type->codegen();
    FunctionType *FT =
        FunctionType::get(functionType, argTypes, false);
    Function *F =
        Function::Create(FT, Function::ExternalLinkage, name, TheModule.get());

    unsigned Idx = 0;
    for (auto &arg : F->args())
        arg.setName(argsAsVars[Idx++]->getName());

    if (!F)
        return nullptr;

    if (!F->empty())
        return LogError<Function>("Function cannot be redefined.");

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
    Builder.SetInsertPoint(BB);
    
    NamedValues.clear();
    for (auto &arg : F->args())
        NamedValues[arg.getName()] = &arg;

    for (int i = 0; i < argsAsVars.size(); ++i) {
        AllocaInst *v = Builder.CreateAlloca(argTypes[i], 0, argsAsVars[i]->getName()); 
    }

    // TODO: CreateAlloca per each local variable

    // TODO: handle function body
    bodyCodegen(routineBody, &this);

    // B ? Builder.CreateRet(B) : Builder.CreateRetVoid();
    verifyFunction(*F);
    return F;
}

// ? *ConstantAST::codegen()
// {
//     return LogError<?>(std::string(__func__) + " not implemented yet");
// }

void IfThenPartAST::codegen(BasicBlock *block)
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void IfAST::codegen(BasicBlock *block)
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void CheckAST::codegen(BasicBlock *block)
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void RaiseAST::codegen(BasicBlock *block)
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void ReturnAST::codegen(BasicBlock *block)
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void BreakAST::codegen(BasicBlock *block)
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void AssignmentAST::codegen(BasicBlock *block)
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void LoopAST::codegen(BasicBlock *block)
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void CatchAST::codegen(BasicBlock *block)
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void TryAST::codegen(BasicBlock *block)
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}
