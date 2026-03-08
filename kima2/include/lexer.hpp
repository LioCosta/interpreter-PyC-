#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <string_view>

namespace Kima {

enum class TokenType {
    // Palavras-chave
    Inicio, Var, Def, Retorna,
    Se, Senao, Enquanto, Para,
    Escreve, Le,
    // Tipos
    Int, Float, String, Bool,
    // Literais
    Verdadeiro, Falso,
    // Operadores lógicos
    E, Ou, Nao,
    // Símbolos
    Ident, Num, FloatNum, StringLit,
    Mais, Menos, Mult, Div, Mod,
    Igual, IgualIgual, Diferente,
    Menor, Maior, MenorIgual, MaiorIgual,
    Atribuicao,
    DoisPontos,
    Virgula,
    AbrePar, FechaPar,
    AbreChave, FechaChave,
    FimArquivo,
    Erro
};

struct Token {
    TokenType tipo;
    std::string_view lexema;
    int linha;
    int coluna;
};

class Lexer {
public:
    explicit Lexer(std::string_view fonte);
    std::vector<Token> tokenizar();

private:
    std::string_view fonte;
    size_t pos = 0;
    int linha = 1;
    int coluna = 1;

    char peek() const;
    char advance();
    bool isAtEnd() const;
    void skipWhitespaceAndComments();
    Token readNumber();
    Token readString();
    Token readIdentifierOrKeyword();
    Token readOperator();
};

} // namespace Kima

#endif