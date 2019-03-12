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

class ObjectAST
{};

class TypeAST : public ObjectAST
{};

class ExpressionAST : public TypeAST
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

class BlockMemberAST
{};

class DefinitionAST
{};

class RoutineDefinitionAST : public DefinitionAST
{};

class Statement
{};

class StatementAST : Statement
{};



