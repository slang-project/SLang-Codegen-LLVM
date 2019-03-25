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

    if (type == "LITERAL_Integer")
    {
        int value = std::stoi(std::string(input["value"].get<std::string>()));
        parsed = new LiteralPrimaryAST(value);
    }
    else if (type == "REFERENCE")
    {
        // TODO: parse identifier correctly
        Identifier name = std::string(input["value"].get<std::string>());
        parsed = new ReferencePrimaryAST(name);
    }
    else if (type == "BINARY")
    {
        //TODO: handle array index exceptions
        ExpressionAST* LHS = parseExpressionAST(input["children"][0]);
        ExpressionAST* RHS = parseExpressionAST(input["children"][1]);
        std::string op = input["value"].get<std::string>();
        parsed = new BinaryExpressionAST(getBinaryOp(op), LHS, RHS);
    }
    else if (type == "CALL")
    {
        throw new std::runtime_error("callExpression not implemented yet");
    }
 
    return parsed;
}

TypeAST* parseTypeAST(const json &input)
{
  TypeAST* parsed = nullptr;
  std::string type{input["type"].get<std::string>()};

  if (type == "type")
  {
    Identifier name = std::string(input["value"].get<std::string>());

    parsed = new UnitRefTypeAST(name);
  }

  return parsed;
}

DeclarationAST* parseDeclarationAST(const json &input)
{
  DeclarationAST* parsed = nullptr; 
  std::string type{input["type"].get<std::string>()};

  if (type == "variableDefinition")
  {
    Identifier name = std::string(input["value"].get<std::string>());
    
    int num_children = int(input["num_children"].get<int>());

    //TODO: handle array index exceptions
    TypeAST* type = parseTypeAST(input["children"][0]);
    ExpressionAST* Body = nullptr;
    if (num_children == 2)
    {
      Body = parseExpressionAST(input["children"][1]);
    }
      
    parsed = new VariableDeclarationAST(name, type, Body);
  }
  else if (type == "routineDefinition")
  {
    Identifier name = std::string(input["value"].get<std::string>());

    json rout_args = input["children"][0];
    int num_args = std::stoi(std::string(rout_args["num_children"].get<std::string>()));
    std::vector<VariableDeclarationAST*> args(num_args);
    
    for (int i = 0; i < num_args; i++)
    {
      std::cout << "Create Vars\n";
      args[i] = (VariableDeclarationAST*) parseDeclarationAST(rout_args["children"][i]);
    }
   
    TypeAST* type = parseTypeAST(input["children"][1]);
    ExpressionAST* expr = parseExpressionAST(input["children"][2]);

    parsed = new RoutineDeclarationAST(name, args, type, expr);
  }

  return parsed;
}


//std::unique_ptr<ObjectAST> parseObjectAST(const json &input)
//{
//    std::unique_ptr<ObjectAST> parsed;
//    std::string type{input["type"].dump()};

//    return parsed;
//}
