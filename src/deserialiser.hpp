#include "entity_hierarchy.hpp"
#include <memory>

BinaryOp getBinaryOp(const std::string& op);

ExpressionAST* parseExpressionAST(const json &input);

ObjectAST* parseObjectAST(const json &input);

DefinitionAST* parseDefinitionAST(const json &input);
