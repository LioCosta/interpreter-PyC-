// lexer.h - corta o código em pedaços
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

// Tipos de token
enum TipoToken {
    NUM,        
    NOME,       // variável
    MAIS,      
    MENOS,      
    MULT,       
    DIV,        // /
    IGUAL,      // =
    IGUAL2,     // ==
    DIF,        // !=
    MENOR,      // <
    MAIOR,      // >
    MENOR_IG,   // <=
    MAIOR_IG,   // >=
    DOIS_PT,    // :
    PRINT,     
    IF,        
    ELSE,       
    WHILE,      
    INPUT,     
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