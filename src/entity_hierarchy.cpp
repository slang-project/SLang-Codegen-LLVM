#include "entity_hierarchy.hpp"


// GLOBAL LLVM CONTAINERS
static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::map<std::string, Value *> NamedValues;


// EXTERNAL INTERFACE
void initLLVMGlobal(std::string moduleName)
{
    TheModule = llvm::make_unique<Module>(moduleName, TheContext);
}

void printGeneratedCode(std::string outFilePath)
{
    std::error_code EC;
    raw_fd_ostream file(outFilePath.c_str(), EC);
    TheModule->print(file, nullptr);
}


// HIERARCHY ------------------------------------------------

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
    return ConstantInt::get(TheContext, APInt(32, std::stoi(value), true));
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
    ReferenceAST *callee = dynamic_cast<ReferenceAST*>(secondary);
    if(!callee)
        return LogError<Value>("Callee not identified.");

    Function *CalleeF = TheModule->getFunction(callee->getName());
    if (!CalleeF)
        return LogError<Value>("Unknown function referenced");

    //TODO: add additional type checks maybe
    if (CalleeF->arg_size() != actuals.size())
        return LogError<Value>("Incorrect number of arguments passed");

    std::vector<Value *> ArgsV;
    for (auto &a : actuals)
    {
        ArgsV.push_back(a->codegen());
        if (!ArgsV.back())
            return nullptr;
    }
    return Builder.CreateCall(CalleeF, ArgsV, "calltmp");
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
    // TODO: create binary operations versions for all type pairs:
    // e.g. this is special version for (int)a ? (int)b
    Value *L = left->codegen();
    Value *R = right->codegen();
    if (!L || !R)
        return nullptr;

    if (binaryOp == "+")
    {
        return Builder.CreateAdd(L, R, "addtmp");
    }
    else if (binaryOp == "-")
    {
        return Builder.CreateSub(L, R, "subtmp");
    }
    else if (binaryOp == "*")
    {
        return Builder.CreateMul(L, R, "multmp");
    }
    else if (binaryOp == "/")
    {
        return Builder.CreateUDiv(L, R, "divtmp");
    }
    else if (binaryOp == "<")
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
/*  if (!F->empty())
        return LogError<Function>("Function cannot be redefined.");
*/
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

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
    Builder.SetInsertPoint(BB);
    
    // NamedValues.clear();
    for (auto &arg : F->args())
        NamedValues[arg.getName()] = &arg;

    // TODO: rewrite to general case
    for (auto &arg : *routineBody)
    {
        if (ReturnAST *retstmt = dynamic_cast<ReturnAST*>(arg))
        {
            ExpressionAST* expr = retstmt->getExpression();
            if (expr)
            {
                Value *retval = expr->codegen();
                if (retval)
                {
                    Builder.CreateRet(retval);
                    verifyFunction(*F, errs());
                    return F;
                }
            }
        }
    }

    // TODO: handle function body
    //bodyCodegen(routineBody, &this);

    // Error reading body, remove function.
    F->eraseFromParent();
    return nullptr;
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
    // B ? Builder.CreateRet(B) : Builder.CreateRetVoid();
    LogError<void>(std::string(__func__) + " not implemented yet");  // TODO
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

Function *CompilationAST::codegen()
{
    return anonymous->codegen();  // TODO
}
