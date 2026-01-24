#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

class Parser {
private:
    Lexer lexer;
    Token currentToken;

    void eat(TipoToken tipo);

    AST* factor();
    AST* term();
    AST* expr();
    AST* statement();

public:
    Parser(Lexer l);
    AST* parse();
};

#endif

