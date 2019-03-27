#include "deserializer.hpp"
#include <functional>
#include <map>
#include <utility>


static std::map<std::string, std::function<EntityAST*(const json&)>> deserializeMapping =
{
    #include "deserializeMapping.data"
};


template<typename T>
std::vector<T> deserializeVector(const json &in)
{
    std::vector<T> container{};
    for (auto &elem : in[CHILDREN])
        container.push_back(dynamic_cast<T>(deserializeMapping[elem[TYPE].get<std::string>()](elem)));
    return container;
}


IdentifierAST deserializeIdentifierAST(const json &in)
{
    return in[VALUE].get<std::string>();
}

ConditionalIfThenPartAST *deserializeConditionalIfThenPartAST(const json &in)
{
    return LogError<ConditionalIfThenPartAST>(std::string(__func__) + " not implemented yet");
}

ConditionalAST *deserializeConditionalAST(const json &in)
{
    return LogError<ConditionalAST>(std::string(__func__) + " not implemented yet");
}

ThisAST *deserializeThisAST(const json &in)
{
    return LogError<ThisAST>(std::string(__func__) + " not implemented yet");
}

ReturnExprAST *deserializeReturnExprAST(const json &in)
{
    return LogError<ReturnExprAST>(std::string(__func__) + " not implemented yet");
}

OldAST *deserializeOldAST(const json &in)
{
    return LogError<OldAST>(std::string(__func__) + " not implemented yet");
}

ReferenceAST *deserializeReferenceAST(const json &in)
{
    return LogError<ReferenceAST>(std::string(__func__) + " not implemented yet");
}

UnresolvedAST *deserializeUnresolvedAST(const json &in)
{
    return LogError<UnresolvedAST>(std::string(__func__) + " not implemented yet");
}

IntegerAST *deserializeIntegerAST(const json &in)
{
    return new IntegerAST
    (
        in[VALUE].get<std::string>()
    );
}

RealAST *deserializeRealAST(const json &in)
{
    return new RealAST
    (
        in[VALUE].get<std::string>()
    );
}

CharacterAST *deserializeCharacterAST(const json &in)
{
    return new CharacterAST
    (
        in[VALUE].get<std::string>()
    );
}

StringAST *deserializeStringAST(const json &in)
{
    return new StringAST
    (
        in[VALUE].get<std::string>()
    );
}

TupleAST *deserializeTupleAST(const json &in)
{
    return LogError<TupleAST>(std::string(__func__) + " not implemented yet");
}

MemberAST *deserializeMemberAST(const json &in)
{
    return LogError<MemberAST>(std::string(__func__) + " not implemented yet");
}

CallAST *deserializeCallAST(const json &in)
{
    return LogError<CallAST>(std::string(__func__) + " not implemented yet");
}
 
UnaryAST *deserializeUnaryAST(const json &in)
{
    return LogError<UnaryAST>(std::string(__func__) + " not implemented yet");
}

BinaryAST *deserializeBinaryAST(const json &in)
{
    auto &inc = in[CHILDREN];
    return new BinaryAST
    (
        in[VALUE],
        dynamic_cast<ExpressionAST*>(deserializeMapping[inc[0][TYPE].get<std::string>()](inc[0])),
        dynamic_cast<ExpressionAST*>(deserializeMapping[inc[1][TYPE].get<std::string>()](inc[1]))
    );
}

UnitRefAST *deserializeUnitRefAST(const json &in)
{
    return new UnitRefAST
    (
        in[VALUE].get<std::string>()
    );
}

MultiTypeAST *deserializeMultiTypeAST(const json &in)
{
    return LogError<MultiTypeAST>(std::string(__func__) + " not implemented yet");
}

RangeTypeAST *deserializeRangeTypeAST(const json &in)
{
    return LogError<RangeTypeAST>(std::string(__func__) + " not implemented yet");
}

VariableAST *deserializeVariableAST(const json &in)
{
    return LogError<VariableAST>(std::string(__func__) + " not implemented yet");
}

UnitAST *deserializeUnitAST(const json &in)
{
    return LogError<UnitAST>(std::string(__func__) + " not implemented yet");
}

RoutineAST *deserializeRoutineAST(const json &in)
{
    auto &inc = in[CHILDREN];
    return new RoutineAST
    (
        deserializeIdentifierAST(inc[0]),
        !inc[1].is_null(),
        deserializeVector<EntityAST*>(inc[2]),
        inc[3].is_null() ? nullptr : dynamic_cast<TypeAST*>(deserializeMapping[inc[3][TYPE].get<std::string>()](inc[3])),
        deserializeVector<ExpressionAST*>(inc[4][CHILDREN][0]),
        !inc[4][VALUE].is_null(),
        inc[5].is_null() ? nullptr : deserializeBodyAST(inc[5]),
        deserializeVector<ExpressionAST*>(inc[6][CHILDREN][0]),
        !inc[6][VALUE].is_null()
    );
}

ConstantAST *deserializeConstantAST(const json &in)
{
    return LogError<ConstantAST>(std::string(__func__) + " not implemented yet");
}

BodyAST *deserializeBodyAST(const json &in)
{
    return new std::vector<EntityAST*>(deserializeVector<EntityAST*>(in));
}

IfThenPartAST *deserializeIfThenPartAST(const json &in)
{
    return LogError<IfThenPartAST>(std::string(__func__) + " not implemented yet");
}

IfAST *deserializeIfAST(const json &in)
{
    return LogError<IfAST>(std::string(__func__) + " not implemented yet");
}

CheckAST *deserializeCheckAST(const json &in)
{
    return LogError<CheckAST>(std::string(__func__) + " not implemented yet");
}

RaiseAST *deserializeRaiseAST(const json &in)
{
    return LogError<RaiseAST>(std::string(__func__) + " not implemented yet");
}

ReturnAST *deserializeReturnAST(const json &in)
{
    auto &inc = in[CHILDREN][0];
    return new ReturnAST
    (
        inc.is_null() ? nullptr : dynamic_cast<ExpressionAST*>(deserializeMapping[inc[TYPE].get<std::string>()](inc))
    );
}

BreakAST *deserializeBreakAST(const json &in)
{
    return LogError<BreakAST>(std::string(__func__) + " not implemented yet");
}

AssignmentAST *deserializeAssignmentAST(const json &in)
{
    return LogError<AssignmentAST>(std::string(__func__) + " not implemented yet");
}

LoopAST *deserializeLoopAST(const json &in)
{
    return LogError<LoopAST>(std::string(__func__) + " not implemented yet");
}

CatchAST *deserializeCatchAST(const json &in)
{
    return LogError<CatchAST>(std::string(__func__) + " not implemented yet");
}

TryAST *deserializeTryAST(const json &in)
{
    return LogError<TryAST>(std::string(__func__) + " not implemented yet");
}

CompilationAST *deserializeCompilationAST(const json &in)
{
    return new CompilationAST
    (
        deserializeRoutineAST(in)
    );
}
