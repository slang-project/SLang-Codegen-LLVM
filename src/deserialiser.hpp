#include "entity_hierarchy.hpp"
#include <memory>

BinaryOp getBinaryOp(const std::string& op);

ExpressionAST* parseExpressionAST(const json &input);

EntityAST* parseEntityAST(const json &input);

DeclarationAST* parseDeclarationAST(const json &input);
