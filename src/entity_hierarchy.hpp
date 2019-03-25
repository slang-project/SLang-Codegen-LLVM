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
Type *LogErrorT(const char *Str);

void initialize();
void print_generated_code();

//--------ENTITIES---------------

class EntityAST
{
  std::vector<EntityAST*> children;
public:
  EntityAST() : children() {}
  void addChild(EntityAST* objectPtr)
  {
    children.push_back(objectPtr);
  }
};

class ExpressionAST : public EntityAST
{
public:
    virtual ~ExpressionAST() {}
    virtual Value *codegen() { return LogErrorV("Use of abstract ExpressionAST."); }
};

////-----------TYPE--------------

class TypeAST : public EntityAST
{
public:
  virtual ~TypeAST() {}
  virtual Type *codegen() { return LogErrorT("Use of abstract TypeAST");}
};

class UnitRefTypeAST : public TypeAST
{
  Identifier name;
public:
  UnitRefTypeAST(const Identifier& name) : name(name) {}
  Type *codegen() override; 
};

class MultiTypeAST : public TypeAST
{};

class RangeTypeAST : public TypeAST
{};

class TupleTypeAST : public TypeAST
{};

class RoutineTypeAST : public TypeAST
{};

////------------DECLARATION------------

class DeclarationAST : public EntityAST
{
public:
    virtual ~DeclarationAST() {}
    virtual Value *codegen() { return LogErrorV("Use of abstract DeclarationAST."); }
};

class VariableDeclarationAST : public DeclarationAST
{
  Identifier Name;
  TypeAST* type;
  ExpressionAST* Body;

public:
  VariableDeclarationAST(const Identifier& Name, TypeAST* type,
                        ExpressionAST* Body)
    : Name(Name), type(type), Body(Body) {}
  
  Value *codegen() override;
  const Identifier &getName() const { return Name; }
  TypeAST* getType() {return type; }
};

class ConstantDeclarationAST : public DeclarationAST
{};

//////-------------UNIT-------------------

class UnitDeclarationAST : public DeclarationAST
{};

class PackageDeclarationAST : public UnitDeclarationAST
{};

//////-------------ROUTINE----------------

class RoutineDeclarationAST : public DeclarationAST
{
  Identifier Name;
  std::vector<VariableDeclarationAST*> Args;
  TypeAST* type;

  // TODO: for simplifying purposes, later should be turned into BlockAST
  ExpressionAST* Body;
public:
  RoutineDeclarationAST(const Identifier& Name, std::vector<VariableDeclarationAST*> Args,
                      TypeAST* type, ExpressionAST* Body)
       : Name(Name), Args(Args), type(type), Body(Body) {}

  Function *codegen();
  const Identifier &getName() const { return Name; }
  TypeAST* getType() {return type; }
};

class InitializerDeclarationAST : public RoutineDeclarationAST
{};

////--------------STATEMENT--------------

class StatementAST : public EntityAST
{};

class IfStatementAST : public StatementAST
{};

class IfThenStatementAST : public StatementAST
{};

class CheckStatementAST : public StatementAST
{};

class RaiseStatementAST : public StatementAST
{};

class ReturnStatementAST : public StatementAST
{};

class BreakStatementAST : public StatementAST
{};

class AssignmentStatementAST : public StatementAST
{};

class LoopStatementAST : public StatementAST
{};

class TryStatementAST : public StatementAST
{};

class CatchStatementAST : public StatementAST
{};

////----------------EXPRESSION---------------

class ConditionIfThenExpressionAST : public ExpressionAST
{};

//////--------------PRIMARY------------------

class PrimaryExpressionAST : public ExpressionAST
{};

class ConditionalPrimaryAST : public PrimaryExpressionAST
{};

class ThisPrimaryAST : public PrimaryExpressionAST
{};

class ReturnPrimaryAST : public PrimaryExpressionAST
{};

class OldPrimaryAST : public PrimaryExpressionAST
{};

class ReferencePrimaryAST : public PrimaryExpressionAST
{
  Identifier Name;
public:
  ReferencePrimaryAST(const Identifier& Name) : Name(Name) {}
  Value *codegen() override;
};

class UnresolvedPrimaryAST : public PrimaryExpressionAST
{
  Identifier Name;
public:
  UnresolvedPrimaryAST(const Identifier& Name) : Name(Name) {}
  Value *codegen() override;
};

class LiteralPrimaryAST : public PrimaryExpressionAST
{
  int Val;  // for now mapped intExpression to LiteralPrimary
public:
  LiteralPrimaryAST(int val) : Val(val) {}
  Value *codegen() override;
};

class TuplePrimaryAST : public PrimaryExpressionAST
{};

//////--------------SECONDARY------------------

class SecondaryExpressionAST : public ExpressionAST
{};

class MemberSecondaryAST : public SecondaryExpressionAST
{};

class CallSecondaryAST : public SecondaryExpressionAST
{
//  Identifier Callee;
//   std::vector<std::unique_ptr<ExpressionAST>> Args;

// public:
//   explicit CallExpressionAST(const std::string &Callee,
//               std::vector<std::unique_ptr<ExpressionAST>> Args)
//       : Callee(Callee), Args(std::move(Args)) {}
//   CallExpressionAST(const CallExpressionAST&) = delete;
//   CallExpressionAST& operator=(const CallExpressionAST&) = delete;
//   ~CallExpressionAST() = default;
//   Value *codegen() override;

};

//////----------------UNARY--------------------

class UnaryExpressionAST : public ExpressionAST
{};

class NewUnaryAST : public UnaryExpressionAST
{};

class InUnaryAST : public UnaryExpressionAST
{};

//////----------------BINARY-----------------

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

class PowerBinaryAST : public BinaryExpressionAST
{};

class MultiplicativeBinaryAST : public BinaryExpressionAST
{};

class AdditiveBinaryAST : public BinaryExpressionAST
{};

class RelationalBinaryAST : public BinaryExpressionAST
{};

class LogicalBinaryAST : public BinaryExpressionAST
{};

