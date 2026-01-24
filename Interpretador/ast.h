#ifndef AST_H
#define AST_H

#include <string>
#include "lexer.h"

// Classe base
class AST {
public:
    virtual ~AST() {}
};

// Número
class NumberNode : public AST {
public:
    std::string value;
    NumberNode(const std::string& val);
};

// Variável
class VarNode : public AST {
public:
    std::string name;
    VarNode(const std::string& n);
};

// Operação binária (+ - * /)
class BinOpNode : public AST {
public:
    AST* left;
    Token op;
    AST* right;

    BinOpNode(AST* l, Token oper, AST* r);
};

// Atribuição: x = expr
class AssignNode : public AST {
public:
    VarNode* left;
    AST* right;

    AssignNode(VarNode* l, AST* r);
};

#endif
