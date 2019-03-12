#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;
using namespace llvm;
using Identifier = std::string; //TODO: create custom type

class ObjectAST
{};

class TypeAST : public ObjectAST
{
    Identifier name;
};

class ExpressionAST : public ObjectAST
{
public:
    virtual ~ExpressionAST() {}
    virtual Value *codegen() = 0;
};

class NumberExpressionAST : public ExpressionAST
{
    double Val;
public:
    NumberExpressionAST(double val) : Val(val) {}
    virtual ~NumberExpressionAST(){}
    virtual Value *codegen();
};

class BlockAST : public ObjectAST
{};

class ModuleAST : public BlockAST
{};

class BlockMemberAST : public ObjectAST
{};

class DefinitionAST : public ObjectAST
{};

class UnitDefinitionAST : public DefinitionAST
{};

class RoutineDefinitionAST : public DefinitionAST
{};

class VariableDefinitionAST : public DefinitionAST
{};

class StatementAST : public ObjectAST
{};

class IfStatementAST : public StatementAST
{};

class LoopStatementAST : public StatementAST
{};

class ReturnStatementAST : public StatementAST
{};

class CallStatementAST : public StatementAST
{};

class AssignmentStatementAST : public StatementAST
{};



