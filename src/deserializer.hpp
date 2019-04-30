#include "entity_hierarchy.hpp"
#include <nlohmann/json.hpp>

#define TYPE "type"
#define VALUE "value"
#define NUM_CHILDREN "num_children"
#define CHILDREN "children"


template<typename T>
std::vector<T> deserializeVector(const json &in);


IdentifierAST deserializeIdentifierAST(const json &in);

ConditionalIfThenPartAST *deserializeConditionalIfThenPartAST(const json &in);

ConditionalAST *deserializeConditionalAST(const json &in);

ThisAST *deserializeThisAST(const json &in);

ReturnExprAST *deserializeReturnExprAST(const json &in);

OldAST *deserializeOldAST(const json &in);

ReferenceAST *deserializeReferenceAST(const json &in);

UnresolvedAST *deserializeUnresolvedAST(const json &in);

IntegerAST *deserializeIntegerAST(const json &in);

RealAST *deserializeRealAST(const json &in);

CharacterAST *deserializeCharacterAST(const json &in);

StringAST *deserializeStringAST(const json &in);

TupleAST *deserializeTupleAST(const json &in);

MemberAST *deserializeMemberAST(const json &in);

CallAST *deserializeCallAST(const json &in);

UnaryAST *deserializeUnaryAST(const json &in);

BinaryAST *deserializeBinaryAST(const json &in);

UnitRefAST *deserializeUnitRefAST(const json &in);

MultiTypeAST *deserializeMultiTypeAST(const json &in);

RangeTypeAST *deserializeRangeTypeAST(const json &in);

VariableAST *deserializeVariableAST(const json &in);

UnitAST *deserializeUnitAST(const json &in);

RoutineAST *deserializeRoutineAST(const json &in);

ConstantAST *deserializeConstantAST(const json &in);

BodyAST *deserializeBodyAST(const json &in);

IfThenPartAST *deserializeIfThenPartAST(const json &in);

IfAST *deserializeIfAST(const json &in);

CheckAST *deserializeCheckAST(const json &in);

RaiseAST *deserializeRaiseAST(const json &in);

ReturnAST *deserializeReturnAST(const json &in);

BreakAST *deserializeBreakAST(const json &in);

AssignmentAST *deserializeAssignmentAST(const json &in);

LoopAST *deserializeLoopAST(const json &in);

CatchAST *deserializeCatchAST(const json &in);

TryAST *deserializeTryAST(const json &in);

CompilationAST *deserializeCompilationAST(const json &in);
