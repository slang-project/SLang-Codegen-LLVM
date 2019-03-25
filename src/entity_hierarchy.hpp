// LLVM INCLUDES
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


// ADDITIONAL INCLUDES
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>


// NAMESPACES SETUP
using json = nlohmann::json;
using IdentifierAST = std::string;
using BodyAST = std::vector<EntityAST>;
using namespace llvm;


// LOGGING
Value *LogErrorV(const char *Str);
Type *LogErrorT(const char *Str);


// EXTERNAL INTERFACE
void initLLVMGlobal();
void printGeneratedCode(std::string outFilePath);


// HIERARCHY
class EntityAST  // abstract
{
public:
    virtual ~EntityAST() = default;
};

class CompilationAST : public EntityAST
{
    // USE_LIST         uses
    // DECLARATION_LIST units_and_standalones
    // ROUTINE          anonymous
public:
    virtual ~CompilationAST() = default;
    virtual Function *codegen();
};

class ExpressionAST : public EntityAST  // abstract
{
    // TYPE type
public:
    virtual ~ExpressionAST() = default;
    virtual Value *codegen();
};

class PrimaryAST : public ExpressionAST  // abstract
{
public:
    virtual ~PrimaryAST() = default;
};

class ConditionalIfThenPartAST : public ExpressionAST
{
    ExpressionAST condition;
    ExpressionAST thenPart;
public:
    ConditionalIfThenPartAST
    (
        ExpressionAST condition,
        ExpressionAST thenPart
    )
      : condition(condition),
        thenPart(thenPart)
    {}
    virtual ~ConditionalIfThenPartAST() = default;
    virtual Value *codegen() override;
};

class ConditionalAST : public PrimaryAST
{
    std::vector<ConditionalIfThenPartAST> ifThenParts;
    ExpressionAST elsePart;
public:
    ConditionalAST
    (
        std::vector<ConditionalIfThenPartAST> ifThenParts,
        ExpressionAST elsePart
    )
      : ifThenParts(ifThenParts),
        elsePart(elsePart)
    {}
    virtual ~ConditionalAST() = default;
    virtual Value *codegen() override;
};

class ThisAST : public PrimaryAST
{
    // UNIT unit
public:
    virtual ~ThisAST() = default;
    virtual Value *codegen() override;
};

class ReturnExprAST : public PrimaryAST
{
    // ROUTINE routine
public:
    virtual ~ReturnExprAST() = default;
    virtual Value *codegen() override;
};

class OldAST : public PrimaryAST
{
    ExpressionAST old;
public:
    OldAST
    (
        ExpressionAST old
    )
      : old(old)
    {}
    virtual ~OldAST() = default;
    virtual Value *codegen() override;
};

class ReferenceAST : public PrimaryAST
{
    // DECLARATION declaration !! changed to:
    IdentifierAST declarationName;  // NOTE: not like in original parser
public:
    ReferenceAST
    (
        IdentifierAST declarationName
    )
      : declarationName(declarationName)
    {}
    virtual ~ReferenceAST() = default;
    virtual Value *codegen() override;
};

// NOTE: SHOULD NOT APPEAR IN FINAL AST! (remove??)
class UnresolvedAST : public PrimaryAST
{
    IdentifierAST name;
public:
    UnresolvedAST
    (
        IdentifierAST name
    )
      : name(name)
    {}
    virtual ~UnresolvedAST() = default;
    virtual Value *codegen() override;
};

// NOTE: abstract here, but not in original parser
class LiteralAST : public PrimaryAST
{
    std::string value;  // value
public:
    LiteralAST
    (
        std::string value
    )
      : value(value)
    {}
    virtual ~LiteralAST() = default;
    virtual Value *codegen() override;
};

// NOTE: absent in original parser
class IntegerAST : public LiteralAST
{
public:
    IntegerAST
    (
        std::string value
    )
      : LiteralAST(value)
    {}
    virtual ~IntegerAST() = default;
    virtual Value *codegen() override;
};

// NOTE: absent in original parser
class RealAST : public LiteralAST
{
public:
    RealAST
    (
        std::string value
    )
      : LiteralAST(value)
    {}
    virtual ~RealAST() = default;
    virtual Value *codegen() override;
};

// NOTE: absent in original parser
class CharacterAST : public LiteralAST
{
public:
    CharacterAST
    (
        std::string value
    )
      : LiteralAST(value)
    {}
    virtual ~CharacterAST() = default;
    virtual Value *codegen() override;
};

// NOTE: absent in original parser
class StringAST : public LiteralAST
{
public:
    StringAST
    (
        std::string value
    )
      : LiteralAST(value)
    {}
    virtual ~StringAST() = default;
    virtual Value *codegen() override;
};

class TupleAST : public PrimaryAST
{
    std::vector<ExpressionAST> expressions;
public:
    TupleAST
    (
        std::vector<ExpressionAST> expressions
    )
      : expressions(expressions)
    {}
    virtual ~TupleAST() = default;
    virtual Value *codegen() override;
};

class SecondaryAST : public ExpressionAST  // indirectly abstract
{
public:
    virtual ~SecondaryAST() = default;
};

// TODO: think about code generation semantics
class MemberAST : public SecondaryAST
{
    ExpressionAST secondary;
    IdentifierAST member;
public:
    MemberAST
    (
        ExpressionAST secondary,
        IdentifierAST member
    )
      : secondary(secondary),
        member(member)
    {}
    virtual ~MemberAST() = default;
    virtual Value *codegen() override;
};

class CallAST : public SecondaryAST
{
    ExpressionAST secondary;
    std::vector<ExpressionAST> actuals;
public:
    CallAST
    (
        ExpressionAST secondary,
        std::vector<ExpressionAST> actuals
    )
      : secondary(secondary),
        actuals(actuals)
    {}
    virtual ~CallAST() = default;
    virtual Value *codegen() override;
};

class UnaryAST : public ExpressionAST
{
    std::string unaryOp;  // value
    ExpressionAST primary;
public:
    UnaryAST
    (
        std::string unaryOp,
        ExpressionAST primary
    )
      : unaryOp(unaryOp),
        primary(primary)
    {}
    virtual ~UnaryAST() = default;
    virtual Value *codegen() override;
};

/* TODO:
class InExprAST : public UnaryAST
{
    // unaryOp, primary -- from parent!
    RangeTypeAST range;
public:
    InExprAST
    (
        std::string unaryOp,
        ExpressionAST primary,
        RangeTypeAST range
    )
      : UnaryAST(unaryOp, primary),
        range(range)
    {}
    virtual ~InExprAST() = default;
    virtual Value *codegen() override;
};
*/

class BinaryAST : public ExpressionAST
{
    std::string binaryOp;  // value, NOTE: not like in original parser
    ExpressionAST left;
    ExpressionAST right;
public:
    BinaryAST
    (
        std::string binaryOp,
        ExpressionAST left,
        ExpressionAST right
    )
      : binaryOp(binaryOp),
        left(left),
        right(right)
    {}
    virtual ~BinaryAST() = default;
    virtual Value *codegen() override;
};

class TypeAST : public EntityAST  // abstract
{
public:
    virtual ~TypeAST() = default;
    virtual Type *codegen();
};

class UnitRefAST : public TypeAST
{
    std::string name;  // value
    // bool opt
    // bool as_sign
    // DECLARATION unit_ref
    // ENTITY_LIST generic_actuals
public:
    UnitRefAST
    (
        std::string name
    )
      : name(name)
    {}
    virtual ~UnitRefAST() = default;
    virtual Type *codegen() override;
};

class MultiTypeAST : public TypeAST
{
    std::vector<UnitRefAST> types;
public:
    MultiTypeAST
    (
        std::vector<UnitRefAST> types
    )
      : types(types)
    {}
    virtual ~MultiTypeAST() = default;
    virtual Type *codegen() override;
};

class RangeTypeAST : public TypeAST
{
    ExpressionAST left;
    ExpressionAST right;
public:
    RangeTypeAST
    (
        ExpressionAST left,
        ExpressionAST right
    )
      : left(left),
        right(right)
    {}
    virtual ~RangeTypeAST() = default;
    virtual Type *codegen() override;
};

/* TODO:
class TupleTypeAST : TypeAST {};
class RoutineTypeAST : TypeAST {};
*/

class DeclarationAST : public EntityAST  // abstract
{
    // bool isHidden
    // bool isFinal
    IdentifierAST name;
public:
    DeclarationAST
    (
        IdentifierAST name
    )
      : name(name)
    {}
    virtual ~DeclarationAST() = default;
};

class VariableAST : public DeclarationAST
{
    // name (isHidden, isFinal) -- from parent!
    // bool isConst
    bool isRef;
    // bool isOverride
    // bool isAbstract
    bool isConcurrent;
    bool isForeign;
    TypeAST type;
    ExpressionAST initializer;
public:
    VariableAST
    (
        IdentifierAST name, 
        bool isRef, 
        bool isConcurrent,
        bool isForeign,
        TypeAST type,
        ExpressionAST initializer
    )
      : DeclarationAST(name),
        isRef(isRef),
        isConcurrent(isConcurrent),
        isForeign(isForeign),
        type(type),
        initializer(initializer)
    {}
    virtual ~VariableAST() = default;
    virtual Value *codegen();
};

class UnitAST : public DeclarationAST
{
    // name (isHidden, isFinal) -- from parent!
    // IDENTIFIER alias
    bool isRef;
    // bool isAbstract
    bool isConcurrent;
    // FORMAL_GENERIC_LIST generics
    // PARENT_LIST inherits
    // USE_LIST uses
    bool isForeign;
    std::vector<DeclarationAST> declarations;
    std::vector<ExpressionAST> invariants;
public:
    UnitAST
    (
        IdentifierAST name,
        bool isRef, 
        bool isConcurrent, 
        bool isForeign, 
        std::vector<DeclarationAST> declarations, 
        std::vector<ExpressionAST> invariants
    )
      : DeclarationAST(name),
        isRef(isRef),
        isConcurrent(isConcurrent),
        isForeign(isForeign),
        declarations(declarations),
        invariants(invariants)
    {}
    virtual ~UnitAST() = default;
    // virtual ? *codegen();  // TODO
};

class RoutineAST : public DeclarationAST
{
    // name (isHidden, isFinal) -- from parent!
    // IDENTIFIER alias
    // PURE_SAFE_SPEC pureSafe
    // bool isAbstract
    bool isForeign;
    // bool isOverride
    // FORMAL_GENERIC_LIST genericParameters
    std::vector<EntityAST> parameters;
    TypeAST type;
    std::vector<ExpressionAST> preconditions;
    bool requireElse;
    BodyAST routineBody;
    std::vector<ExpressionAST> postconditions;
    bool ensureThen;
public:
    RoutineAST
    (
        IdentifierAST name,
        bool isForeign,
        std::vector<EntityAST> parameters,
        TypeAST type,
        std::vector<ExpressionAST> preconditions,
        bool requireElse,
        BodyAST routineBody,
        std::vector<ExpressionAST> postconditions,
        bool ensureThen
    )
      : DeclarationAST(name),
        isForeign(isForeign),
        parameters(parameters),
        type(type),
        preconditions(preconditions),
        requireElse(requireElse),
        routineBody(routineBody),
        postconditions(postconditions),
        ensureThen(ensureThen)
    {}
    virtual ~RoutineAST() = default;
    virtual Function *codegen();
};

class ConstantAST : public DeclarationAST
{
    std::vector<ExpressionAST> constants;
public:
    ConstantAST
    (
        std::vector<ExpressionAST> constants
    )
      : DeclarationAST(""),
        constants(constants)
    {}
    virtual ~ConstantAST() = default;
    // virtual ? *codegen();  // TODO
};

class StatementAST : public EntityAST  // abstract
{
public:
    virtual ~StatementAST() = default;
    virtual void codegen(BasicBlock *block);
};

class IfThenPartAST : public StatementAST
{
    ExpressionAST condition;
    BodyAST thenPart;
public:
    IfThenPartAST
    (
        ExpressionAST condition,
        BodyAST thenPart
    )
      : condition(condition),
        thenPart(thenPart)
    {}
    virtual ~IfThenPartAST() = default;
    virtual void codegen(BasicBlock *block) override;
};

class IfAST : public StatementAST
{
    std::vector<IfThenPartAST> ifThenParts;
    BodyAST elsePart;
public:
    IfAST
    (
        std::vector<IfThenPartAST> ifThenParts,
        BodyAST elsePart
    )
      : ifThenParts(ifThenParts),
        elsePart(elsePart)
    {}
    virtual ~IfAST() = default;
    virtual void codegen(BasicBlock *block) override;
};

class CheckAST : public StatementAST
{
    std::vector<ExpressionAST> predicates;
public:
    CheckAST
    (
        std::vector<ExpressionAST> predicates
    )
      : predicates(predicates)
    {}
    virtual ~CheckAST() = default;
    virtual void codegen(BasicBlock *block) override;
};

class RaiseAST : public StatementAST
{
    ExpressionAST expression;
public:
    RaiseAST
    (
        ExpressionAST expression
    )
      : expression(expression)
    {}
    virtual ~RaiseAST() = default;
    virtual void codegen(BasicBlock *block) override;
};

class ReturnAST : public StatementAST
{
    ExpressionAST expression;
public:
    ReturnAST
    (
        ExpressionAST expression
    )
      : expression(expression)
    {}
    virtual ~ReturnAST() = default;
    virtual void codegen(BasicBlock *block) override;
};

class BreakAST : public StatementAST
{
    std::string label;  // value
    // STATEMENT labeled
public:
    BreakAST
    (
        std::string label
    )
      : label(label)
    {}
    virtual ~BreakAST() = default;
    virtual void codegen(BasicBlock *block) override;
    
};

class AssignmentAST : public StatementAST
{
    ExpressionAST left;
    ExpressionAST right;
public:
    AssignmentAST
    (
        ExpressionAST left,
        ExpressionAST right
    )
      : left(left),
        right(right)
    {}
    virtual ~AssignmentAST() = default;
    virtual void codegen(BasicBlock *block) override;
};

class LoopAST : public StatementAST
{
    bool prefix;  // value
    VariableAST loopCounter;
    ExpressionAST whileClause;
    std::vector<ExpressionAST> invariants;
    BodyAST body;
    std::vector<ExpressionAST> variants;
public:
    LoopAST
    (
        bool prefix,
        VariableAST loopCounter,
        ExpressionAST whileClause,
        std::vector<ExpressionAST> invariants,
        BodyAST body,
        std::vector<ExpressionAST> variants   
    )
      : prefix(prefix),
        loopCounter(loopCounter),
        whileClause(whileClause),
        invariants(invariants),
        body(body),
        variants(variants)
    {}
    virtual ~LoopAST() = default;
    virtual void codegen(BasicBlock *block) override;
};

class CatchAST : public StatementAST
{
    VariableAST catchVar;
    UnitRefAST unitRef;
    BodyAST body;
public:
    CatchAST
    (
        VariableAST catchVar,
        UnitRefAST unitRef,
        BodyAST body
    )
      : catchVar(catchVar),
        unitRef(unitRef),
        body(body)
    {}
    virtual ~CatchAST() = default;
    virtual void codegen(BasicBlock *block) override;
};

class TryAST : public StatementAST
{
    BodyAST body;  // NOTE: in original parser it is ENTITY_LIST
    std::vector<CatchAST> handlers;
    BodyAST elsePart;  // NOTE: in original parser it is ENTITY_LIST
public:
    TryAST
    (
        BodyAST body,
        std::vector<CatchAST> handlers,
        BodyAST elsePart
    )
      : body(body),
        handlers(handlers),
        elsePart(elsePart)
    {}
    virtual ~TryAST() = default;
    virtual void codegen(BasicBlock *block) override;
};

