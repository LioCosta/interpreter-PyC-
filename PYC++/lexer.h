// lexer.h - corta o código em pedaços
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

// Tipos de token
enum TipoToken {
    NUM,        // número
    NOME,       // variável
    MAIS,       // +
    MENOS,      // -
    MULT,       // *
    DIV,        // /
    IGUAL,      // =
    IGUAL2,     // ==
    DIF,        // !=
    MENOR,      // <
    MAIOR,      // >
    MENOR_IG,   // <=
    MAIOR_IG,   // >=
    DOIS_PT,    // :
    PRINT,      // print
    IF,         // if
    ELSE,       // else
    WHILE,      // while
    INPUT,      // input
    FIM,        // acabou
    ERRO        // erro
};

struct Token {
    TipoToken tipo;
    std::string txt;
    int linha;
};

class Lexer {
private:
    std::string codigo;
    int pos;
    int linha;
    
    char prox();
    char avancar();
    
public:
    Lexer(const std::string& txt);
    
    static std::string lerArq(const std::string& nome);
    
    std::vector<Token> tokenizar();
};

#endif