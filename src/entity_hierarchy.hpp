#include "llvm/ADT/APInt.h"
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

#include "utility.hpp"

#include <nlohmann/json.hpp>
#include <iostream>
#include <string>

using json = nlohmann::json;
using namespace llvm;
using Identifier = std::string; //TODO: create custom type


Value *LogErrorV(const char *Str);

void initialize();
void print_generated_code();

//--------Entities---------------

class ObjectAST
{
  std::vector<ObjectAST*> children;
public:
  ObjectAST() : children() {}
  void addChild(ObjectAST* objectPtr)
  {
    children.push_back(objectPtr);
  }
};

class TypeAST : public ObjectAST
{
    Identifier name;
};

class ExpressionAST : public ObjectAST
{
public:
    virtual ~ExpressionAST() {}
    virtual Value *codegen() { return LogErrorV("Use of abstract ExpressionAST."); }
};

class IntegerExpressionAST : public ExpressionAST
{
    int Val;
public:
    IntegerExpressionAST(int val) : Val(val) {}
    Value *codegen() override;
};

class VariableExpressionAST : public ExpressionAST
{
    Identifier Name;
public:
    VariableExpressionAST(const Identifier& Name) : Name(Name) {}
    Value *codegen() override;
};

enum class BinaryOp
{
  add, subtract, multiply, less_than
};

class BinaryExpressionAST : public ExpressionAST {
  BinaryOp Op;
  ExpressionAST *LHS, *RHS; // TODO: make unique_ptr

public:
  BinaryExpressionAST(BinaryOp Op, ExpressionAST* LHS,
                ExpressionAST* RHS)
      : Op(Op), LHS(LHS), RHS(RHS) {}
  Value *codegen() override;
};

// class CallExpressionAST : public ExpressionAST {
//   Identifier Callee;
//   std::vector<std::unique_ptr<ExpressionAST>> Args;

// public:
//   explicit CallExpressionAST(const std::string &Callee,
//               std::vector<std::unique_ptr<ExpressionAST>> Args)
//       : Callee(Callee), Args(std::move(Args)) {}
//   CallExpressionAST(const CallExpressionAST&) = delete;
//   CallExpressionAST& operator=(const CallExpressionAST&) = delete;
//   ~CallExpressionAST() = default;
//   Value *codegen() override;
// };

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



