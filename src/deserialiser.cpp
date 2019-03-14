#include "deserialiser.hpp"

// TODO: consider refactoring to optimize
BinaryOp getBinaryOp(const std::string &op)
{
    if (op == "+")
        return BinaryOp::add;
    if (op == "-")
        return BinaryOp::subtract;
    if (op == "*")
        return BinaryOp::multiply;
    if (op == "<")
        return BinaryOp::less_than;
    throw new std::runtime_error("Unknown operator resolution.");
}

ExpressionAST* parseExpressionAST(const json &input)
{
    ExpressionAST* parsed = nullptr;
    std::string type{input["type"].get<std::string>()};

    if (type == "integerExpression")
    {
        int value = std::stoi(std::string(input["value"].get<std::string>()));
        parsed = new IntegerExpressionAST(value);
    }
    else if (type == "variableExpression")
    {
        // TODO: parse identifier correctly
        Identifier name = std::string(input["value"].get<std::string>());
        parsed = new VariableExpressionAST(name);
    }
    else if (type == "binaryExpression")
    {
        //TODO: handle array index exceptions
        ExpressionAST* LHS = parseExpressionAST(input["children"][0]);
        ExpressionAST* RHS = parseExpressionAST(input["children"][1]);
        std::string op = input["value"].get<std::string>();
        parsed = new BinaryExpressionAST(getBinaryOp(op), LHS, RHS);
    }
    else if (type == "callExpression")
    {
        throw new std::runtime_error("callExpression not implemented yet");
    }
    return parsed;
}

//std::unique_ptr<ObjectAST> parseObjectAST(const json &input)
//{
//    std::unique_ptr<ObjectAST> parsed;
//    std::string type{input["type"].dump()};

//    return parsed;
//}
