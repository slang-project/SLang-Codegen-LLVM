#include "entity_hierarchy.hpp"
#include <sstream>

// INTERNAL NEEDS CONTAINERS

enum class variableType
{
    AllocaVar,
    ValueVar
};

static std::map<std::string, std::pair<variableType, Value*>> NamedValues;


// EXTERNAL INTERFACE

std::string mangleRoutineName(const std::string &name)
{
    return isLibcName(name) ? "$" + name : name;
}

// HIERARCHY ------------------------------------------------

Value *ConditionalIfThenPartAST::codegen() const
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *ConditionalAST::codegen() const
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *ThisAST::codegen() const
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *ReturnExprAST::codegen() const
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *OldAST::codegen() const
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *ReferenceAST::codegen() const
{
    if (NamedValues.find(declarationName) == NamedValues.end())
        return LogError<Value>("Variable used as REFERENCE was not found.");

    if (NamedValues[declarationName].first == variableType::ValueVar)
        return NamedValues[declarationName].second;

    Value * const ref = NamedValues[declarationName].second;
    if (!ref)
        return LogError<Value>("Unknown variable referenced.");

    return Builder.CreateLoad(ref);
}

Value *LiteralAST::codegen() const
{
    UnitRefAST * const ureftype = dynamic_cast<UnitRefAST*>(type);
    if (!ureftype)
        return LogError<Value>("Literal type is not UnitRef.");
    const std::string typeName = ureftype->getName();

    if (typeName == "Integer")
    {
        IntegerType * const intType = static_cast<IntegerType*>(getLLVMType("Integer"));
        assert(intType);
        uint64_t intVal;
        std::istringstream valStr(value);
        valStr >> intVal;
        return ConstantInt::get(TheContext, APInt(
            intType->getBitWidth(),
            intVal,
            false)); // TODO: get sign from intType
    }

    if (typeName == "Real")
    {
        return ConstantFP::get(TheContext, APFloat(std::stod(value)));
    }

    if (typeName == "Character")
    {
        if (value.length() != 1)
            return LogError<Value>("Character value is too long.");
        IntegerType * const charType = static_cast<IntegerType*>(getLLVMType("Character"));
        assert(charType);
        return ConstantInt::get(TheContext, APInt(
            charType->getBitWidth(),
            value[0],
            false)); // TODO: get sign from charType
    }

    if (typeName == "String")
    {
        return LogError<Value>("String literal is not supported yet.");
    }

    return LogError<Value>("Literal type is not supported.");
}

Value *TupleAST::codegen() const
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

// TODO: think about code generation semantics
Value *MemberAST::codegen() const
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Value *CallAST::codegen() const
{
    const ReferenceAST * const callee = dynamic_cast<ReferenceAST*>(secondary);
    if (!callee)
        return LogError<Value>("Callee not identified.");

    // TODO: check if ReferenceAST is of Routine type

    Function * const CalleeF = TheModule->getFunction(callee->getName());
    if (!CalleeF)
        return LogError<Value>("Unknown function referenced");

    // TODO: add additional type checks maybe
    if (CalleeF->arg_size() != actuals.size())
        return LogError<Value>("Incorrect number of arguments passed");

    std::vector<Value*> ArgsV;
    for (const auto &a : actuals)
    {
        ArgsV.push_back(a->codegen());
        if (!ArgsV.back())
            return nullptr;
    }

    return Builder.CreateCall(CalleeF, ArgsV);
}

Value *UnaryAST::codegen() const
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

/* TODO:
Value *InExprAST::codegen() const
{}
*/

Value *BinaryAST::codegen() const
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
        return Builder.CreateICmpSLT(L, R, "lttmp");
    if (binaryOp == ">")
        return Builder.CreateICmpSGT(L, R, "gttmp");
    if (binaryOp == "==")
        return Builder.CreateICmpEQ(L, R, "eqtmp");

    return LogError<Value>("Unsupported binary operator.");
}

Type *UnitRefAST::codegen() const
{
    Type *res = getLLVMType(name);
    if (!res)
        return LogError<Type>("Type was not defined: " + name);
    return res;
}

Value *UnitRefAST::getDefault() const
{
    Value *res = getLLVMDefaultValue(name);
    if (!res)
        return LogError<Value>("Type was not defined: " + name);
    return res;
}

Type *MultiTypeAST::codegen() const
{
    return LogError<Type>(std::string(__func__) + " not implemented yet");
}

Value *MultiTypeAST::getDefault() const
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

Type *RangeTypeAST::codegen() const
{
    return LogError<Type>(std::string(__func__) + " not implemented yet");
}

Value *RangeTypeAST::getDefault() const
{
    return LogError<Value>(std::string(__func__) + " not implemented yet");
}

/* TODO:
Type *TupleTypeAST::codegen() const
{};

Type *RoutineTypeAST::codegen() const
{};
*/

bool VariableAST::codegen() const
{
    Type *T;
    Value *init;
    Function * const F = Builder.GetInsertBlock()->getParent();

    if (!type)
        return LogError<bool>("Type inference not implemented yet");

    T = type->codegen();
    if (!T)
        return false;

    init = type->getDefault();
    if (!init)
        return false;

    AllocaInst * const Alloca = CreateEntryBlockAlloca(F, T, name);
    NamedValues[name] = { variableType::AllocaVar, Alloca };

    if (initializer)
    {
        init = initializer->codegen();
        if (!init)
            return false;
    }

    return /*(bool)*/ Builder.CreateStore(init, Alloca);
}

bool UnitAST::codegen() const
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool RoutineAST::codegen() const
{
/*  if (!F->empty())  // find routine with this name
        return LogError<Function>("Routine cannot be redefined.");
*/
    // Vector for routine arguments
    std::vector<Type*> argTypes {};
    std::vector<VariableAST*> argsAsVars {};

    for (const auto &arg : parameters)
    {
        VariableAST * const curParam = dynamic_cast<VariableAST*>(arg);
        if (!curParam)
            return LogError<Function>("Parameter is not variable declaration.");

        Type *argType = curParam->getType()->codegen();
        if (!argType)
            return false;  // TODO: log?

        argTypes.push_back(argType);
        argsAsVars.push_back(curParam);
    }

    // Routine type
    Type * const functionType = type ? type->codegen() : Type::getVoidTy(TheContext);
    FunctionType * const FT =
        FunctionType::get(functionType, argTypes, false);
    Function * const F =
        Function::Create(FT, Function::ExternalLinkage, name, TheModule.get());

    size_t Idx = 0;
    for (auto &arg : F->args())
        arg.setName(argsAsVars[Idx++]->getName());

    BasicBlock *BB = BasicBlock::Create(TheContext, "entry", F);
    Builder.SetInsertPoint(BB);

    NamedValues.clear();
    Idx = 0;
    for (auto &arg : F->args())
    {
        NamedValues[arg.getName()] = { variableType::ValueVar, &arg };
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

// ? *ConstantAST::codegen() const
// {
//     return LogError<?>(std::string(__func__) + " not implemented yet");
// }

// TODO: fulfill with gotos like return, break, continue, goto etc.
bool isControlFlow(const EntityAST * const e)
{
    return dynamic_cast<const ReturnAST*>(e);
}

bool BodyAST::codegen() const
{
    for (const auto &arg : body)
    {
        if (const StatementAST *statement = dynamic_cast<StatementAST*>(arg))
        {
            if (!statement->codegen())
                return false;
            if (isControlFlow(statement))
                goto end;  // skip unreachable statements
        }

        else if (const VariableAST *var = dynamic_cast<VariableAST*>(arg))
        {
            if (!var->codegen())
                return false;
        }

        else if (const CallAST *call = dynamic_cast<CallAST*>(arg))
        {
            // TODO: check (return value of codegen?)
            if (!call->codegen())
                return false;
        }

        else return LogError<bool>("Unsupported body element");
    }
    end:
    return true;
}

bool IfThenPartAST::codegen() const
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool IfAST::codegen() const
{
    const ExpressionAST * const Cond = ifThenParts[0]->condition;  // TODO: change
    Value *CondV = Cond->codegen();
    if (!CondV)
        return false;

    // TODO: add cast to Boolean

    Function * const TheFunction = Builder.GetInsertBlock()->getParent();

    // Create blocks for the then and else cases.  Insert the 'then' block at the
    // end of the function.
    BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then", TheFunction);
    BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else");
    BasicBlock *MergeBB = BasicBlock::Create(TheContext, "ifcont");

    Builder.CreateCondBr(CondV, ThenBB, ElseBB);

    // Emit then value.
    Builder.SetInsertPoint(ThenBB);

    const BodyAST * const Then = ifThenParts[0]->thenPart;  // TODO: change
    if (!Then->codegen())
        return false;

    Builder.CreateBr(MergeBB);
    // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
    ThenBB = Builder.GetInsertBlock();

    // Emit else block.
    TheFunction->getBasicBlockList().push_back(ElseBB);
    Builder.SetInsertPoint(ElseBB);

    if (!elsePart->codegen())
        return false;

    Builder.CreateBr(MergeBB);
    // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
    ElseBB = Builder.GetInsertBlock();

    // Emit merge block.
    TheFunction->getBasicBlockList().push_back(MergeBB);
    Builder.SetInsertPoint(MergeBB);
    return true;
}

bool CheckAST::codegen() const
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool RaiseAST::codegen() const
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool ReturnAST::codegen() const
{
    if (!expression)
        return /*(bool)*/ Builder.CreateRetVoid();

    Value * const ret = expression->codegen();
    if (!ret)
        return LogError<bool>("Failed to codegen return expression");

    return /*(bool)*/ Builder.CreateRet(ret);
}

bool BreakAST::codegen() const
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool AssignmentAST::codegen() const
{
    const ReferenceAST * const var = dynamic_cast<ReferenceAST*>(left);
    if (!var)
        return LogError<bool>("LHS of the assignment is not a REFERENCE.");

    if (NamedValues.find(var->getName()) == NamedValues.end())
        return LogError<bool>("Assignemt to the undefined variable");

    if (NamedValues[var->getName()].first == variableType::ValueVar)
        return LogError<bool>("Assignment to the non-reference value.");

    Value * const Alloca = NamedValues[var->getName()].second;

    Value * const R = right->codegen();
    if (!R)
        return LogError<bool>("No expression given to assign.");

    return /*(bool)*/ Builder.CreateStore(R, Alloca);
}

bool LoopAST::codegen() const  // TODO: review
{
    // TODO: loop counter

    // TODO: add cast to Boolean

    Function * const TheFunction = Builder.GetInsertBlock()->getParent();
    BasicBlock *CondBB =
                   BasicBlock::Create(TheContext, "loopcond", TheFunction);
    BasicBlock *LoopBB = BasicBlock::Create(TheContext, "loop");
    BasicBlock * const EndBB = BasicBlock::Create(TheContext, "endloop");

    // CONDITION
    Builder.CreateBr(prefix ? CondBB : LoopBB);

    Builder.SetInsertPoint(CondBB);

    Value *CondV = whileClause->codegen();
    if (!CondV)
        return false;

    Builder.CreateCondBr(CondV, LoopBB, EndBB);

    // BODY
    TheFunction->getBasicBlockList().push_back(LoopBB);

    Builder.SetInsertPoint(LoopBB);

    if (!body->codegen())
        return false;

    LoopBB = Builder.GetInsertBlock();

    Builder.CreateBr(CondBB);

    // END
    TheFunction->getBasicBlockList().push_back(EndBB);

    Builder.SetInsertPoint(EndBB);

    return true;
}

bool CatchAST::codegen() const
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool TryAST::codegen() const
{
    return LogError<bool>(std::string(__func__) + " not implemented yet");
}

bool CompilationAST::codegen() const
{
    for (const auto &decl : units_and_standalones)
    {
        decl->codegen();
    }
    // TODO: check if startup function is required

    if (!anonymous->codegen())
    {
        return LogError<bool>("Anonymous routine generation failed");
    }

    if (!generateStartupRoutine(anonymous->name))
    {
        return LogError<bool>("Startup routine generation failed");
    }

    return true;
}
