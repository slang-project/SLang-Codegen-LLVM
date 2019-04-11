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

    return Builder.CreateLoad(Alloca);
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
    if (!L)
        return nullptr;

    Value *R = right->codegen();
    if (!R)
        return nullptr;

    if (binaryOp == "+")
        return Builder.CreateAdd(L, R, "addtmp");
    if (binaryOp == "-")
        return Builder.CreateSub(L, R, "subtmp");
    if (binaryOp == "*")
        return Builder.CreateMul(L, R, "multmp");
    if (binaryOp == "/")
        return Builder.CreateUDiv(L, R, "divtmp");
    if (binaryOp == "<")
        return Builder.CreateICmpULT(L, R, "cmptmp");

    return LogError<Value>("Unsupported binary operator.");
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
    if (name == "Real")
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

bool VariableAST::codegen()
{ 
    Type *T;
    Value *init;
    Function *F = Builder.GetInsertBlock()->getParent();

    if (!type)
        return LogError<bool>("Type inference not implemented yet");
    
    T = type->codegen();
    if (!T)
        return false;

    init = type->getDefault();
    if (!init)
        return false;

    AllocaInst *Alloca = CreateEntryBlockAlloca(F, T, name);
    NamedValues[name] = Alloca;

    if (initializer)
    {
        init = initializer->codegen();
        if (!init)
            return false;
    }

    return /*(bool)*/ Builder.CreateStore(init, Alloca);
}

Type *UnitAST::codegen()
{
    return LogError<Type>(std::string(__func__) + " not implemented yet");
}

Function *RoutineAST::codegen()
{
/*  if (!F->empty())  // find routine with this name
        return LogError<Function>("Routine cannot be redefined.");
*/
    // Vector for routine arguments
    std::vector<Type*> argTypes{};
    std::vector<VariableAST*> argsAsVars{};

    for (auto &arg : parameters)
    {
        VariableAST *curParam = dynamic_cast<VariableAST*>(arg);
        if (!curParam)
            return LogError<Function>("Parameter is not variable declaration.");

        Type *argType = curParam->getType()->codegen();
        if (!argType)
            return nullptr;  // TODO: log?

        argTypes.push_back(argType);
        argsAsVars.push_back(curParam);
    }

    // Routine type
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

    if (!routineBody->codegen())
    {
        F->eraseFromParent();
        return LogError<Function>("Failed to generate routine body");
    }

    if (verifyFunction(*F, &errs()))
    {
        F->eraseFromParent();
        return LogError<Function>("Failed to verify routine body");
    }

    return F;
}

// ? *ConstantAST::codegen()
// {
//     return LogError<?>(std::string(__func__) + " not implemented yet");
// }

bool BodyAST::codegen()
{
    for (auto &arg : body)
    {
        if (StatementAST *statement = dynamic_cast<StatementAST*>(arg))
        {
            if (!statement->codegen())
                return false;
            continue;
        }

        if (VariableAST *var = dynamic_cast<VariableAST*>(arg))
        {
            if (!var->codegen())
                return false;
            continue;
        }

        if (CallAST *call = dynamic_cast<CallAST*>(arg))
        {
            // TODO: check (return value of codegen?)
            if (!call->codegen())
                return false;
            continue;
        }

        return LogError<bool>("Unsupported body element");
    }
    return true;
}

bool IfThenPartAST::codegen()
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool IfAST::codegen()
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool CheckAST::codegen()
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool RaiseAST::codegen()
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool ReturnAST::codegen()
{
    if (!expression)
        return /*(bool)*/ Builder.CreateRetVoid();

    Value *ret = expression->codegen();
    if (!ret)
        return LogError<bool>("Failed to codegen return expression");

    return /*(bool)*/ Builder.CreateRet(ret);
}

bool BreakAST::codegen()
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool AssignmentAST::codegen()
{
    ReferenceAST *var = dynamic_cast<ReferenceAST*>(left);
    if (!var)
        return LogError<bool>("LHS of the assignment is not a REFERENCE.");

    AllocaInst *Alloca = NamedValues[var->getName()];
    if (!Alloca)
        return LogError<bool>("Assignemt to the undefined variable");

    Value *R = right->codegen();
    if (!R)
        return LogError<bool>("No expression given to assign");

    return /*(bool)*/ Builder.CreateStore(R, Alloca);
}

bool LoopAST::codegen()
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool CatchAST::codegen()
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool TryAST::codegen()
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool CompilationAST::codegen()
{
    return anonymous->codegen();
}
