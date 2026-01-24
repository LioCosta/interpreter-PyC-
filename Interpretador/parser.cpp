#include "parser.h"
#include <iostream>
#include <cstdlib>

Parser::Parser(Lexer l) : lexer(l) {
    currentToken = lexer.PegarToken();
}

void Parser::eat(TipoToken tipo) {
    if (currentToken.type == tipo)
        currentToken = lexer.PegarToken();
    else {
        std::cerr << "Erro de sintaxe\n";
        exit(1);
    }
}

AST* Parser::factor() {
    Token token = currentToken;

    if (token.type == TipoToken::Numero) {
        eat(TipoToken::Numero);
        return new NumberNode(token.value);
    }

    if (token.type == TipoToken::Identificador) {
        eat(TipoToken::Identificador);
        return new VarNode(token.value);
    }

    if (token.type == TipoToken::Lparent) {
        eat(TipoToken::Lparent);
        AST* node = expr();
        eat(TipoToken::Rparent);
        return node;
    }

    std::cerr << "Erro em factor\n";
    exit(1);
}

AST* Parser::term() {
    AST* node = factor();

    while (currentToken.type == TipoToken::Vezes ||
           currentToken.type == TipoToken::Barra) {

        Token token = currentToken;

        if (token.type == TipoToken::Vezes)
            eat(TipoToken::Vezes);
        else
            eat(TipoToken::Barra);

        node = new BinOpNode(node, token, factor());
    }

    return node;
}

AST* Parser::expr() {
    AST* node = term();

    while (currentToken.type == TipoToken::Mais ||
           currentToken.type == TipoToken::Menos) {

        Token token = currentToken;

        if (token.type == TipoToken::Mais)
            eat(TipoToken::Mais);
        else
            eat(TipoToken::Menos);

        node = new BinOpNode(node, token, term());
    }

    return node;
}

AST* Parser::statement() {
    if (currentToken.type == TipoToken::Identificador) {
        Token id = currentToken;
        eat(TipoToken::Identificador);

        if (currentToken.type == TipoToken::Igual) {
            eat(TipoToken::Igual);
            return new AssignNode(
                new VarNode(id.value),
                expr()
            );
        }
    }

    return expr();
}

AST* Parser::parse() {
    return statement();
}

