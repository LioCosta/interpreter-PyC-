#include "lexer.h"
#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& input)
    : text(input), pos(0), char_atual(input[0]) {}

void Lexer::avanco() {
    pos++;
    if (pos < text.size())
        char_atual = text[pos];
    else
        char_atual = '\0';
}

void Lexer::barra_espaco() {
    while (char_atual != '\0' && isspace(char_atual))
        avanco();
}

Token Lexer::Numero() {
    std::string result;
    while (isdigit(char_atual)) {
        result += char_atual;
        avanco();
    }
    return {TipoToken::Numero, result};
}

Token Lexer::Identificador() {
    std::string result;
    while (isalnum(char_atual) || char_atual == '_') {
        result += char_atual;
        avanco();
    }

    if (result == "if" || result == "else")
        return {TipoToken::Pl_chave, result};

    return {TipoToken::Identificador, result};
}

Token Lexer::PegarToken() {
    while (char_atual != '\0') {

        if (isspace(char_atual)) {
            barra_espaco();
            continue;
        }

        if (isdigit(char_atual))
            return Numero();

        if (isalpha(char_atual) || char_atual == '_')
            return Identificador();

        if (char_atual == '+') { avanco(); return {TipoToken::Mais, "+"}; }
        if (char_atual == '-') { avanco(); return {TipoToken::Menos, "-"}; }
        if (char_atual == '*') { avanco(); return {TipoToken::Vezes, "*"}; }
        if (char_atual == '/') { avanco(); return {TipoToken::Barra, "/"}; }
        if (char_atual == '=') { avanco(); return {TipoToken::Igual, "="}; }
        if (char_atual == '(') { avanco(); return {TipoToken::Lparent, "("}; }
        if (char_atual == ')') { avanco(); return {TipoToken::Rparent, ")"}; }

        std::cerr << "Erro: caractere inválido: " << char_atual << std::endl;
        exit(1);
    }

    return {TipoToken::Fim, ""};
}

