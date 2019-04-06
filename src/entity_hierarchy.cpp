#include "entity_hierarchy.hpp"


// GLOBAL LLVM CONTAINERS
static LLVMContext TheContext;
static IRBuilder<> Builder(TheContext);
static std::unique_ptr<Module> TheModule;
static std::map<std::string, AllocaInst*> NamedValues;


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

static AllocaInst *CreateEntryBlockAlloca(Function* TheFunction,
                                          Type* type,
                                          const std::string &VarName)
{
    IRBuilder<> TmpB(&TheFunction->getEntryBlock(),
                     TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(type, 0, VarName.c_str());
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
    AllocaInst *Alloca = NamedValues[declarationName];
    if (!Alloca)
        return LogError<Value>("Unknown variable referenced");
    
    Value *value = Builder.CreateLoad(Alloca);
    return value;
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
    if (!callee)
        return LogError<Value>("Callee not identified.");

    Function *CalleeF = TheModule->getFunction(callee->getName());
    if (!CalleeF)
        return LogError<Value>("Unknown function referenced");

    // TODO: add additional type checks maybe
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

Value *UnitRefAST::getDefault()
{
    if (name == "Integer")
        return ConstantInt::get(TheContext, APInt(32, 0, true));
    else if (name == "Real")
        return ConstantFP::get(TheContext, APFloat(0.0));
    if (name == "Character")
        return LogError<Value>("Charater not implemented yet.");
    if (name == "String")
        return LogError<Value>("String not implemented yet.");
    return LogError<Value>("Invaid type.");
}


Type *MultiTypeAST::codegen()
{
    return LogError<Type>(std::string(__func__) + " not implemented yet");
}

Value *MultiTypeAST::getDefault()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Type *RangeTypeAST::codegen()
{
    return LogError<Type>(std::string(__func__) + " not implemented yet");
}

Value *RangeTypeAST::getDefault()
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}


/* TODO:
Type *TupleTypeAST::codegen
{};

Type *RoutineTypeAST::codegen
{};
*/

Value *VariableAST::codegen()
{ 
    Function *F = Builder.GetInsertBlock()->getParent();
    
    Type *T;
    Value *init;
    if (type)
    {
        T = type->codegen();
        if (!T)
            return nullptr;
        init = type->getDefault();
        if (!init)
            return nullptr;
    }
    else
    {
        return LogError<Value>("type inference not implemented yet");
    }

    AllocaInst *Alloca = CreateEntryBlockAlloca(F, T, name);
    NamedValues[name] = Alloca;

    if (initializer)
    {
        init = initializer->codegen();
        if (!init)
            return nullptr;
    }

    Builder.CreateStore(init, Alloca);
    return init;
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
    
    NamedValues.clear();
    Idx = 0;
    for (auto &arg : F->args())
    {
        AllocaInst *Alloca = CreateEntryBlockAlloca(F, argTypes[Idx++], arg.getName());
        NamedValues[arg.getName()] = Alloca;
    }

    routineBody->codegen();

    // TODO: check for logical correctness
    verifyFunction(*F, &errs());
    return F;

    // Error reading body, remove function.
    F->eraseFromParent();
    return nullptr;
}

// ? *ConstantAST::codegen()
// {
//     return LogError<?>(std::string(__func__) + " not implemented yet");
// }

void BodyAST::codegen()
{
    for (auto &arg : body)
    {
        if (VariableAST *var = dynamic_cast<VariableAST*>(arg))
        {  
            var->codegen(); 
        }

        if (AssignmentAST *ass = dynamic_cast<AssignmentAST*>(arg))
        {
            ass->codegen();
        }

        if (ReturnAST *retstmt = dynamic_cast<ReturnAST*>(arg))
        {
            ExpressionAST* expr = retstmt->getExpression();
            if (expr)
            {
                Value *retval = expr->codegen();
                if (retval)
                {
                    Builder.CreateRet(retval);
                    return;
                }
            }
        }
    }
}

void IfThenPartAST::codegen()
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void IfAST::codegen()
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void CheckAST::codegen()
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void RaiseAST::codegen()
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void ReturnAST::codegen()
{
    // B ? Builder.CreateRet(B) : Builder.CreateRetVoid();
    LogError<void>(std::string(__func__) + " not implemented yet");  // TODO
}

void BreakAST::codegen()
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void AssignmentAST::codegen()
{
    if (ReferenceAST *var = dynamic_cast<ReferenceAST*>(left))
    {
        AllocaInst *Alloca = NamedValues[var->getName()];
        if (!Alloca)
        {
            LogError<void>("Assignemt to the undefined variable");
            return;
        }

        Value *R = right->codegen();
        if (!R)
        {
            LogError<void>("No expression given to assign");
            return;
        }

        Builder.CreateStore(R, Alloca);
    }
    else
    {
        LogError<void>("LHS of the assignment is not a REFERENCE.");
        return;
    }
}

void LoopAST::codegen()
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void CatchAST::codegen()
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void TryAST::codegen()
{
    LogError<void>(std::string(__func__) + " not implemented yet");
}

void CompilationAST::codegen()
{
    anonymous->codegen();
}
