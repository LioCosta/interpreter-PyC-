#ifndef LEXER_H
#define LEXER_H

#include <string>

enum class TipoToken {
    Identificador,
    Numero,
    Mais,
    Menos,
    Vezes,
    Dividir,
    Igual,
    Lparent,
    Rparent,
    Barra,
    Pl_chave,
    Fim
};

struct Token {
    TipoToken type;
    std::string value;
};

class Lexer {
private:
    std::string text;
    size_t pos;
    char char_atual;

public:
    Lexer(const std::string& input);

    void avanco();
    void barra_espaco();

    Token Numero();
    Token Identificador();
    Token PegarToken();
};

#endif

