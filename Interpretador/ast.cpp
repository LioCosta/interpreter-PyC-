#include "ast.h"

NumberNode::NumberNode(const std::string& val)
    : value(val) {}

VarNode::VarNode(const std::string& n)
    : name(n) {}

BinOpNode::BinOpNode(AST* l, Token oper, AST* r)
    : left(l), op(oper), right(r) {}

AssignNode::AssignNode(VarNode* l, AST* r)
    : left(l), right(r) {}

