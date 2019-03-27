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
#include <vector>


// NAMESPACES SETUP
using json = nlohmann::json;
using namespace llvm;


// FORWARD DECLARATION NEEDS
class ExpressionAST;


// EXTERNAL INTERFACE
void initLLVMGlobal(std::string moduleName);
void printGeneratedCode(std::string outFilePath);


// LOGGING
template<typename T>
T *LogError(const char *str)
{
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
}

template<typename T>
T *LogError(std::string str)
{
    return LogError<T>(str.c_str());
}

// HIERARCHY ------------------------------------------------

class EntityAST  // abstract
{
public:
    virtual ~EntityAST() = default;
};

using IdentifierAST = std::string;

using BodyAST = std::vector<EntityAST*>;

class TypeAST : public EntityAST  // abstract
{
protected:
public:
    virtual ~TypeAST() = default;
    virtual Type *codegen() { return LogError<Type>(std::string(__func__) + ": use of abstract class"); }
};

class UnitRefAST : public TypeAST
{
protected:
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
protected:
    std::vector<UnitRefAST*> types;
public:
    MultiTypeAST
    (
        std::vector<UnitRefAST*> types
    )
      : types(types)
    {}
    virtual ~MultiTypeAST() = default;
    virtual Type *codegen() override;
};

class RangeTypeAST : public TypeAST
{
protected:
    ExpressionAST *left;
    ExpressionAST *right;
public:
    RangeTypeAST
    (
        ExpressionAST *left,
        ExpressionAST *right
    )
      : left(left),
        right(right)
    {}
    virtual ~RangeTypeAST() = default;
    virtual Type *codegen() override;
};

/* TODO:
class TupleTypeAST : public TypeAST {};
class RoutineTypeAST : public TypeAST {};
*/

class ExpressionAST : public EntityAST  // abstract
{
protected:
    // TYPE type !! changed to:
    UnitRefAST *type = nullptr;  // NOTE: not like in original parser, but own computed
public:
    virtual ~ExpressionAST() = default;
    virtual Value *codegen() { return LogError<Value>(std::string(__func__) + ": use of abstract class"); }
};

class PrimaryAST : public ExpressionAST  // abstract
{
protected:
public:
    virtual ~PrimaryAST() = default;
};

class ConditionalIfThenPartAST : public ExpressionAST
{
protected:
    ExpressionAST *condition;
    ExpressionAST *thenPart;
public:
    ConditionalIfThenPartAST
    (
        ExpressionAST *condition,
        ExpressionAST *thenPart
    )
      : condition(condition),
        thenPart(thenPart)
    {}
    virtual ~ConditionalIfThenPartAST() = default;
    virtual Value *codegen() override;
};

class ConditionalAST : public PrimaryAST
{
protected:
    std::vector<ConditionalIfThenPartAST*> ifThenParts;
    ExpressionAST * elsePart;
public:
    ConditionalAST
    (
        std::vector<ConditionalIfThenPartAST *> ifThenParts,
        ExpressionAST *elsePart
    )
      : ifThenParts(ifThenParts),
        elsePart(elsePart)
    {}
    virtual ~ConditionalAST() = default;
    virtual Value *codegen() override;
};

class ThisAST : public PrimaryAST
{
protected:
    // UNIT unit
public:
    virtual ~ThisAST() = default;
    virtual Value *codegen() override;
};

class ReturnExprAST : public PrimaryAST
{
protected:
    // ROUTINE routine
public:
    virtual ~ReturnExprAST() = default;
    virtual Value *codegen() override;
};

class OldAST : public PrimaryAST
{
protected:
    ExpressionAST *old;
public:
    OldAST
    (
        ExpressionAST *old
    )
      : old(old)
    {}
    virtual ~OldAST() = default;
    virtual Value *codegen() override;
};

class ReferenceAST : public PrimaryAST
{
protected:
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
    IdentifierAST getName() { return declarationName; }
};

// NOTE: SHOULD NOT APPEAR IN FINAL AST! (remove??)
class UnresolvedAST : public PrimaryAST
{
protected:
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
class LiteralAST : public PrimaryAST  // indirectly abstract
{
protected:
    std::string value;  // value
public:
    LiteralAST
    (
        std::string value
    )
      : value(value)
    {}
    virtual ~LiteralAST() = default;
};

// NOTE: absent in original parser
class IntegerAST : public LiteralAST
{
protected:
public:
    IntegerAST
    (
        std::string value
    )
      : LiteralAST(value)
    {
        this->type = new UnitRefAST("Integer");
    }
    virtual ~IntegerAST() = default;
    virtual Value *codegen() override;
};

// NOTE: absent in original parser
class RealAST : public LiteralAST
{
protected:
public:
    RealAST
    (
        std::string value
    )
      : LiteralAST(value)
    {
        this->type = new UnitRefAST("Real");
    }
    virtual ~RealAST() = default;
    virtual Value *codegen() override;
};

// NOTE: absent in original parser
class CharacterAST : public LiteralAST
{
protected:
public:
    CharacterAST
    (
        std::string value
    )
      : LiteralAST(value)
    {
        this->type = new UnitRefAST("Character");
    }
    virtual ~CharacterAST() = default;
    virtual Value *codegen() override;
};

// NOTE: absent in original parser
class StringAST : public LiteralAST
{
protected:
public:
    StringAST
    (
        std::string value
    )
      : LiteralAST(value)
    {
        this->type = new UnitRefAST("String");
    }
    virtual ~StringAST() = default;
    virtual Value *codegen() override;
};

class TupleAST : public PrimaryAST
{
protected:
    std::vector<ExpressionAST*> expressions;
public:
    TupleAST
    (
        std::vector<ExpressionAST*> expressions
    )
      : expressions(expressions)
    {}
    virtual ~TupleAST() = default;
    virtual Value *codegen() override;
};

class SecondaryAST : public ExpressionAST  // indirectly abstract
{
protected:
public:
    virtual ~SecondaryAST() = default;
};

// TODO: think about code generation semantics
class MemberAST : public SecondaryAST
{
protected:
    ExpressionAST *secondary;
    IdentifierAST member;
public:
    MemberAST
    (
        ExpressionAST *secondary,
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
protected:
    ExpressionAST *secondary;
    std::vector<ExpressionAST*> actuals;
public:
    CallAST
    (
        ExpressionAST *secondary,
        std::vector<ExpressionAST*> actuals
    )
      : secondary(secondary),
        actuals(actuals)
    {}
    virtual ~CallAST() = default;
    virtual Value *codegen() override;
};

class UnaryAST : public ExpressionAST
{
protected:
    std::string unaryOp;  // value
    ExpressionAST *primary;
public:
    UnaryAST
    (
        std::string unaryOp,
        ExpressionAST *primary
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
protected:
    // unaryOp, primary -- from parent!
    RangeTypeAST *range;
public:
    InExprAST
    (
        std::string unaryOp,
        ExpressionAST *primary,
        RangeTypeAST *range
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
protected:
    std::string binaryOp;  // value, NOTE: not like in original parser
    ExpressionAST *left;
    ExpressionAST *right;
public:
    BinaryAST
    (
        std::string binaryOp,
        ExpressionAST *left,
        ExpressionAST *right
    )
      : binaryOp(binaryOp),
        left(left),
        right(right)
    {}
    virtual ~BinaryAST() = default;
    virtual Value *codegen() override;
};

class DeclarationAST : public EntityAST  // abstract
{
protected:
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
protected:
    // name (isHidden, isFinal) -- from parent!
    // bool isConst
    bool isRef;
    // bool isOverride
    // bool isAbstract
    bool isConcurrent;
    bool isForeign;
    TypeAST *type;
    ExpressionAST *initializer;
public:
    VariableAST
    (
        IdentifierAST name, 
        bool isRef, 
        bool isConcurrent,
        bool isForeign,
        TypeAST *type,
        ExpressionAST *initializer
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
    TypeAST *getType() { return type; }
    IdentifierAST getName() { return name; }
};

class UnitAST : public DeclarationAST
{
protected:
    // name (isHidden, isFinal) -- from parent!
    // IDENTIFIER alias
    bool isRef;
    // bool isAbstract
    bool isConcurrent;
    // FORMAL_GENERIC_LIST generics
    // PARENT_LIST inherits
    // USE_LIST uses
    bool isForeign;
    std::vector<DeclarationAST*> declarations;
    std::vector<ExpressionAST*> invariants;
public:
    UnitAST
    (
        IdentifierAST name,
        bool isRef, 
        bool isConcurrent, 
        bool isForeign, 
        std::vector<DeclarationAST*> declarations, 
        std::vector<ExpressionAST*> invariants
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
    friend class CompilationAST;  // for anonymous function
protected:
    // name (isHidden, isFinal) -- from parent!
    // IDENTIFIER alias
    // PURE_SAFE_SPEC pureSafe
    // bool isAbstract
    bool isForeign;
    // bool isOverride
    // FORMAL_GENERIC_LIST genericParameters
    std::vector<EntityAST*> parameters;
    TypeAST* type;
    std::vector<ExpressionAST*> preconditions;
    bool requireElse;
    BodyAST *routineBody;
    std::vector<ExpressionAST*> postconditions;
    bool ensureThen;
public:
    RoutineAST
    (
        IdentifierAST name,
        bool isForeign,
        std::vector<EntityAST*> parameters,
        TypeAST* type,
        std::vector<ExpressionAST*> preconditions,
        bool requireElse,
        BodyAST *routineBody,
        std::vector<ExpressionAST*> postconditions,
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
protected:
    std::vector<ExpressionAST*> constants;
public:
    ConstantAST
    (
        std::vector<ExpressionAST*> constants
    )
      : DeclarationAST(""),
        constants(constants)
    {}
    virtual ~ConstantAST() = default;
    // virtual ? *codegen();  // TODO
};

class StatementAST : public EntityAST  // abstract
{
protected:
public:
    virtual ~StatementAST() = default;
    virtual void codegen(BasicBlock *block) { LogError<void>(std::string(__func__) + ": use of abstract class"); }
};

class IfThenPartAST : public StatementAST
{
protected:
    ExpressionAST *condition;
    BodyAST thenPart;
public:
    IfThenPartAST
    (
        ExpressionAST *condition,
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
protected:
    std::vector<IfThenPartAST*> ifThenParts;
    BodyAST elsePart;
public:
    IfAST
    (
        std::vector<IfThenPartAST*> ifThenParts,
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
protected:
    std::vector<ExpressionAST*> predicates;
public:
    CheckAST
    (
        std::vector<ExpressionAST*> predicates
    )
      : predicates(predicates)
    {}
    virtual ~CheckAST() = default;
    virtual void codegen(BasicBlock *block) override;
};

class RaiseAST : public StatementAST
{
protected:
    ExpressionAST* expression;
public:
    RaiseAST
    (
        ExpressionAST* expression
    )
      : expression(expression)
    {}
    virtual ~RaiseAST() = default;
    virtual void codegen(BasicBlock *block) override;
};

class ReturnAST : public StatementAST
{
protected:
    ExpressionAST* expression;
public:
    ReturnAST
    (
        ExpressionAST* expression
    )
      : expression(expression)
    {}
    virtual ~ReturnAST() = default;
    virtual void codegen(BasicBlock *block) override;
    ExpressionAST *getExpression() { return expression; }
};

class BreakAST : public StatementAST
{
protected:
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
protected:
    ExpressionAST* left;
    ExpressionAST* right;
public:
    AssignmentAST
    (
        ExpressionAST* left,
        ExpressionAST* right
    )
      : left(left),
        right(right)
    {}
    virtual ~AssignmentAST() = default;
    virtual void codegen(BasicBlock *block) override;
};

class LoopAST : public StatementAST
{
protected:
    bool prefix;  // value
    VariableAST* loopCounter;
    ExpressionAST* whileClause;
    std::vector<ExpressionAST*> invariants;
    BodyAST body;
    std::vector<ExpressionAST*> variants;
public:
    LoopAST
    (
        bool prefix,
        VariableAST* loopCounter,
        ExpressionAST* whileClause,
        std::vector<ExpressionAST*> invariants,
        BodyAST body,
        std::vector<ExpressionAST*> variants   
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
protected:
    VariableAST* catchVar;
    UnitRefAST* unitRef;
    BodyAST body;
public:
    CatchAST
    (
        VariableAST* catchVar,
        UnitRefAST* unitRef,
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
protected:
    BodyAST body;  // NOTE: in original parser it is ENTITY_LIST
    std::vector<CatchAST*> handlers;
    BodyAST elsePart;  // NOTE: in original parser it is ENTITY_LIST
public:
    TryAST
    (
        BodyAST body,
        std::vector<CatchAST*> handlers,
        BodyAST elsePart
    )
      : body(body),
        handlers(handlers),
        elsePart(elsePart)
    {}
    virtual ~TryAST() = default;
    virtual void codegen(BasicBlock *block) override;
};

class CompilationAST : public EntityAST
{
protected:
    // USE_LIST         uses
    // DECLARATION_LIST units_and_standalones
    RoutineAST *anonymous;
public:
    CompilationAST
    (
        RoutineAST *anonymous
    )
      : anonymous(anonymous)
    {
        // NOTE: temporary changes for compatibility with linker of GCC
        anonymous->name = "main";
        anonymous->type = new UnitRefAST("Integer");
    }
    virtual ~CompilationAST() = default;
    virtual Function *codegen();
};
