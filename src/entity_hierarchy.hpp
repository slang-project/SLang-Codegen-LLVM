#include "llvm_utils.hpp"  // TheModule, Builder, TheContext

#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <vector>


// NAMESPACES SETUP
using json = nlohmann::json;


// FORWARD DECLARATION NEEDS

class BodyAST;
class ExpressionAST;


// LOGGING

template<typename T>
T *LogError(const char * const str)
{
    fprintf(stderr, "Error: %s\n", str);
    return nullptr;
}

template<typename T>
T *LogError(const std::string &str)
{
    return LogError<T>(str.c_str());
}

template<bool>
bool LogError(const std::string &str)
{
    LogError<void>(str.c_str());
    return false;
}

/// Mangle the given name in case if it is the same as libc name.
std::string mangleRoutineName(const std::string &name);

// HIERARCHY ------------------------------------------------

class EntityAST  // abstract
{
public:
    virtual ~EntityAST() = default;
};

// NOTE: there is no need in separate container for the IdentifierAST
using IdentifierAST = std::string;

class TypeAST : public EntityAST  // abstract
{
protected:
public:
    virtual ~TypeAST() = default;
    virtual Type *codegen() const
    {
        return LogError<Type>(std::string(__func__) + ": use of abstract class");
    }
    virtual Value *getDefault() const
    {
        return LogError<Value>(std::string(__func__) + ": use of abstract class");
    }
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
    virtual Type *codegen() const override;
    virtual Value *getDefault() const override;
    IdentifierAST getName() const
    {
        return name;
    }
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
    virtual Type *codegen() const override;
    virtual Value *getDefault() const override;
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
    virtual Type *codegen() const override;
    virtual Value *getDefault() const override;
};

/* TODO:
class TupleTypeAST : public TypeAST {};
class RoutineTypeAST : public TypeAST {};
*/

class ExpressionAST : public EntityAST  // abstract
{
protected:
    TypeAST *type;
    ExpressionAST
    (
        TypeAST *type
    )
      : type(type)
    {}
public:
    virtual ~ExpressionAST() = default;
    virtual Value *codegen() const
    {
        return LogError<Value>(std::string(__func__) + ": use of abstract class");
    }
};

class PrimaryAST : public ExpressionAST  // abstract
{
protected:
    PrimaryAST
    (
        TypeAST *type
    )
      : ExpressionAST(type)
    {}
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
        TypeAST *type,
        ExpressionAST *condition,
        ExpressionAST *thenPart
    )
      : ExpressionAST(type),
        condition(condition),
        thenPart(thenPart)
    {}
    virtual ~ConditionalIfThenPartAST() = default;
    virtual Value *codegen() const override;
};

class ConditionalAST : public PrimaryAST
{
protected:
    std::vector<ConditionalIfThenPartAST*> ifThenParts;
    ExpressionAST *elsePart;
public:
    ConditionalAST
    (
        TypeAST *type,
        std::vector<ConditionalIfThenPartAST *> ifThenParts,
        ExpressionAST *elsePart
    )
      : PrimaryAST(type),
        ifThenParts(ifThenParts),
        elsePart(elsePart)
    {}
    virtual ~ConditionalAST() = default;
    virtual Value *codegen() const override;
};

class ThisAST : public PrimaryAST
{
protected:
    // UNIT unit
public:
    virtual ~ThisAST() = default;
    virtual Value *codegen() const override;
};

class ReturnExprAST : public PrimaryAST
{
protected:
    // ROUTINE routine
public:
    virtual ~ReturnExprAST() = default;
    virtual Value *codegen() const override;
};

class OldAST : public PrimaryAST
{
protected:
    ExpressionAST *old;
public:
    OldAST
    (
        TypeAST *type,
        ExpressionAST *old
    )
      : PrimaryAST(type),
        old(old)
    {}
    virtual ~OldAST() = default;
    virtual Value *codegen() const override;
};

class ReferenceAST : public PrimaryAST
{
    friend class CallAST; // for name mangling
protected:
    // DECLARATION declaration !! changed to:
    IdentifierAST declarationName;  // NOTE: not like in original parser
public:
    ReferenceAST
    (
        TypeAST *type,
        IdentifierAST declarationName
    )
      : PrimaryAST(type),
        declarationName(declarationName)
    {}
    virtual ~ReferenceAST() = default;
    virtual Value *codegen() const override;
    IdentifierAST getName() const
    {
        return declarationName;
    }
};

// NOTE: THIS TYPE SHOULD NOT BE PRESENT IN THE FINAL IR!
class UnresolvedAST : public PrimaryAST
{
protected:
    IdentifierAST name;
private:
    // NOTE: Objects of this type must not be created!
    UnresolvedAST
    (
        TypeAST *type,
        IdentifierAST name
    )
      : PrimaryAST(type),
        name(name)
    {}
public:
    virtual ~UnresolvedAST() = default;
    // virtual Value *codegen() const override;
};

class LiteralAST : public PrimaryAST
{
protected:
    std::string value;  // value
public:
    LiteralAST
    (
        std::string value,
        TypeAST *type
    )
      : PrimaryAST(type),
        value(value)
    {}
    virtual ~LiteralAST() = default;
    virtual Value *codegen() const override;
};

class TupleAST : public PrimaryAST
{
protected:
    std::vector<ExpressionAST*> expressions;
public:
    TupleAST
    (
        TypeAST *type,
        std::vector<ExpressionAST*> expressions
    )
      : PrimaryAST(type),
        expressions(expressions)
    {}
    virtual ~TupleAST() = default;
    virtual Value *codegen() const override;
};

class SecondaryAST : public ExpressionAST  // indirectly abstract
{
protected:
public:
    SecondaryAST
    (
        TypeAST* type
    )
      : ExpressionAST(type)
    {}
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
        TypeAST *type,
        ExpressionAST *secondary,
        IdentifierAST member
    )
      : SecondaryAST(type),
        secondary(secondary),
        member(member)
    {}
    virtual ~MemberAST() = default;
    virtual Value *codegen() const override;
};

class CallAST : public SecondaryAST
{
protected:
    ExpressionAST *secondary;
    std::vector<ExpressionAST*> actuals;
public:
    CallAST
    (
        TypeAST *type,
        ExpressionAST *secondary,
        std::vector<ExpressionAST*> actuals
    )
      : SecondaryAST(type),
        secondary(secondary),
        actuals(actuals)
    {
        ReferenceAST *sec = dynamic_cast<ReferenceAST*>(secondary);
        if (!sec)
        {
            throw "Only REFERENCE objects may be called.";
        }
        sec->declarationName = mangleRoutineName(sec->declarationName);
    }
    virtual ~CallAST() = default;
    virtual Value *codegen() const override;
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
        TypeAST *type,
        ExpressionAST *primary
    )
      : ExpressionAST(type),
        unaryOp(unaryOp),
        primary(primary)
    {}
    virtual ~UnaryAST() = default;
    virtual Value *codegen() const override;
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
        TypeAST *type,
        ExpressionAST *primary,
        RangeTypeAST *range
    )
      : UnaryAST(unaryOp, type, primary),
        range(range)
    {}
    virtual ~InExprAST() = default;
    virtual Value *codegen() const override;
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
        TypeAST *type,
        ExpressionAST *left,
        ExpressionAST *right
    )
      : ExpressionAST(type),
        binaryOp(binaryOp),
        left(left),
        right(right)
    {}
    virtual ~BinaryAST() = default;
    virtual Value *codegen() const override;
};

class DeclarationAST : public EntityAST  // abstract
{
protected:
    // bool isHidden
    // bool isFinal
    IdentifierAST name;
    DeclarationAST
    (
        IdentifierAST name
    )
      : name(name)
    {}
public:
    virtual ~DeclarationAST() = default;
    virtual bool codegen() const = 0;
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
    virtual bool codegen() const;  // TODO: think about case of unit member
    TypeAST *getType() const
    {
        return type;
    }
    IdentifierAST getName() const
    {
        return name;
    }
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
    virtual bool codegen() const;
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
    TypeAST *type;
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
        TypeAST *type,
        std::vector<ExpressionAST*> preconditions,
        bool requireElse,
        BodyAST *routineBody,
        std::vector<ExpressionAST*> postconditions,
        bool ensureThen
    )
      : DeclarationAST(mangleRoutineName(name)),
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
    virtual bool codegen() const;
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
    virtual bool codegen() const;  // TODO
};

class StatementAST : public EntityAST  // abstract
{
protected:
public:
    virtual ~StatementAST() = default;
    virtual bool codegen() const
    {
        return LogError<bool>(std::string(__func__) + ": use of abstract class");
    }
};

class BodyAST : public StatementAST
{
protected:
    std::vector<EntityAST*> body;
public:
    BodyAST
    (
        std::vector<EntityAST*> body
    )
      : body(body)
    {}
    virtual ~BodyAST() = default;
    virtual bool codegen() const override;
};

class IfThenPartAST : public StatementAST
{
    friend class IfAST;  // TODO: remove?
protected:
    ExpressionAST *condition;
    BodyAST *thenPart;
public:
    IfThenPartAST
    (
        ExpressionAST *condition,
        BodyAST *thenPart
    )
      : condition(condition),
        thenPart(thenPart)
    {}
    virtual ~IfThenPartAST() = default;
    virtual bool codegen() const override;
};

class IfAST : public StatementAST
{
protected:
    std::vector<IfThenPartAST*> ifThenParts;
    BodyAST *elsePart;
public:
    IfAST
    (
        std::vector<IfThenPartAST*> ifThenParts,
        BodyAST *elsePart
    )
      : ifThenParts(ifThenParts),
        elsePart(elsePart)
    {}
    virtual ~IfAST() = default;
    virtual bool codegen() const override;
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
    virtual bool codegen() const override;
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
    virtual bool codegen() const override;
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
    virtual bool codegen() const override;
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
    virtual bool codegen() const override;
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
    virtual bool codegen() const override;
};

class LoopAST : public StatementAST
{
protected:
    bool prefix;  // value
    VariableAST* loopCounter;
    ExpressionAST* whileClause;
    std::vector<ExpressionAST*> invariants;
    BodyAST *body;
    std::vector<ExpressionAST*> variants;
public:
    LoopAST
    (
        bool prefix,
        VariableAST* loopCounter,
        ExpressionAST* whileClause,
        std::vector<ExpressionAST*> invariants,
        BodyAST *body,
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
    virtual bool codegen() const override;
};

class CatchAST : public StatementAST
{
protected:
    VariableAST* catchVar;
    UnitRefAST* unitRef;
    BodyAST *body;
public:
    CatchAST
    (
        VariableAST* catchVar,
        UnitRefAST* unitRef,
        BodyAST *body
    )
      : catchVar(catchVar),
        unitRef(unitRef),
        body(body)
    {}
    virtual ~CatchAST() = default;
    virtual bool codegen() const override;
};

class TryAST : public StatementAST
{
protected:
    BodyAST *body;  // NOTE: in original parser it is ENTITY_LIST
    std::vector<CatchAST*> handlers;
    BodyAST *elsePart;  // NOTE: in original parser it is ENTITY_LIST
public:
    TryAST
    (
        BodyAST *body,
        std::vector<CatchAST*> handlers,
        BodyAST *elsePart
    )
      : body(body),
        handlers(handlers),
        elsePart(elsePart)
    {}
    virtual ~TryAST() = default;
    virtual bool codegen() const override;
};

class CompilationAST : public EntityAST
{
protected:
    // USE_LIST         uses
    std::vector<DeclarationAST*> units_and_standalones;
    RoutineAST *anonymous;
public:
    CompilationAST
    (
        std::vector<DeclarationAST*> units_and_standalones,
        RoutineAST *anonymous
    )
      : units_and_standalones(units_and_standalones),
        anonymous(anonymous)
    {}
    virtual ~CompilationAST() = default;
    virtual bool codegen() const;
};
