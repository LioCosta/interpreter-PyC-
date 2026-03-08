#include "lexer.hpp"
#include <cctype>
#include <stdexcept>
#include <unordered_map>

namespace Kima {

Lexer::Lexer(std::string_view fonte) : fonte(fonte) {}

char Lexer::peek() const {
    return isAtEnd() ? '\0' : fonte[pos];
}

char Lexer::advance() {
    char c = peek();
    if (c == '\n') {
        linha++;
        coluna = 1;
    } else {
        coluna++;
    }
    pos++;
    return c;
}

bool Lexer::isAtEnd() const {
    return pos >= fonte.size();
}

void Lexer::skipWhitespaceAndComments() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r') {
            advance();
        } else if (c == '\n') {
            advance();
        } else if (c == '/' && pos + 1 < fonte.size() && fonte[pos+1] == '/') {
            // comentário até fim da linha
            while (!isAtEnd() && peek() != '\n') advance();
        } else {
            break;
        }
    }
}

Token Lexer::readNumber() {
    size_t start = pos;
    int startCol = coluna;
    while (isdigit(peek())) advance();
    if (peek() == '.') {
        advance();
        while (isdigit(peek())) advance();
        return Token{TokenType::FloatNum, fonte.substr(start, pos - start), linha, startCol};
    }
    return Token{TokenType::Num, fonte.substr(start, pos - start), linha, startCol};
}

Token Lexer::readString() {
    advance(); // consome "
    size_t start = pos;
    int startCol = coluna;
    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\\') advance(); // escape simples
        advance();
    }
    if (isAtEnd()) throw std::runtime_error("String não fechada");
    std::string_view conteudo = fonte.substr(start, pos - start);
    advance(); // consome "
    return Token{TokenType::StringLit, conteudo, linha, startCol};
}

Token Lexer::readIdentifierOrKeyword() {
    size_t start = pos;
    int startCol = coluna;
    while (isalnum(peek()) || peek() == '_') advance();
    std::string_view texto = fonte.substr(start, pos - start);

    static const std::unordered_map<std::string_view, TokenType> keywords = {
        {"inicio", TokenType::Inicio},
        {"var", TokenType::Var},
        {"def", TokenType::Def},
        {"retorna", TokenType::Retorna},
        {"se", TokenType::Se},
        {"senao", TokenType::Senao},
        {"enquanto", TokenType::Enquanto},
        {"para", TokenType::Para},
        {"escreve", TokenType::Escreve},
        {"le", TokenType::Le},
        {"inteiro", TokenType::Int},
        {"decimal", TokenType::Float},
        {"texto", TokenType::String},
        {"logico", TokenType::Bool},
        {"verdadeiro", TokenType::Verdadeiro},
        {"falso", TokenType::Falso},
        {"e", TokenType::E},
        {"ou", TokenType::Ou},
        {"nao", TokenType::Nao}
    };
    auto it = keywords.find(texto);
    if (it != keywords.end())
        return Token{it->second, texto, linha, startCol};
    return Token{TokenType::Ident, texto, linha, startCol};
}

Token Lexer::readOperator() {
    char c = advance();
    int startCol = coluna - 1;
    // Operadores de dois caracteres
    if (c == '=' && peek() == '=') { advance(); return Token{TokenType::IgualIgual, "==", linha, startCol}; }
    if (c == '!' && peek() == '=') { advance(); return Token{TokenType::Diferente, "!=", linha, startCol}; }
    if (c == '<' && peek() == '=') { advance(); return Token{TokenType::MenorIgual, "<=", linha, startCol}; }
    if (c == '>' && peek() == '=') { advance(); return Token{TokenType::MaiorIgual, ">=", linha, startCol}; }
    if (c == '&' && peek() == '&') { advance(); return Token{TokenType::E, "&&", linha, startCol}; }
    if (c == '|' && peek() == '|') { advance(); return Token{TokenType::Ou, "||", linha, startCol}; }

    // Operadores de um caractere
    switch (c) {
        case '+': return Token{TokenType::Mais, "+", linha, startCol};
        case '-': return Token{TokenType::Menos, "-", linha, startCol};
        case '*': return Token{TokenType::Mult, "*", linha, startCol};
        case '/': return Token{TokenType::Div, "/", linha, startCol};
        case '%': return Token{TokenType::Mod, "%", linha, startCol};
        case '=': return Token{TokenType::Atribuicao, "=", linha, startCol};
        case '<': return Token{TokenType::Menor, "<", linha, startCol};
        case '>': return Token{TokenType::Maior, ">", linha, startCol};
        case ':': return Token{TokenType::DoisPontos, ":", linha, startCol};
        case ',': return Token{TokenType::Virgula, ",", linha, startCol};
        case '(': return Token{TokenType::AbrePar, "(", linha, startCol};
        case ')': return Token{TokenType::FechaPar, ")", linha, startCol};
        case '{': return Token{TokenType::AbreChave, "{", linha, startCol};
        case '}': return Token{TokenType::FechaChave, "}", linha, startCol};
        default: return Token{TokenType::Erro, std::string_view(&c, 1), linha, startCol};
    }
}

std::vector<Token> Lexer::tokenizar() {
    std::vector<Token> tokens;
    while (!isAtEnd()) {
        skipWhitespaceAndComments();
        if (isAtEnd()) break;

        char c = peek();
        if (isdigit(c)) {
            tokens.push_back(readNumber());
        } else if (isalpha(c) || c == '_') {
            tokens.push_back(readIdentifierOrKeyword());
        } else if (c == '"') {
            tokens.push_back(readString());
        } else {
            tokens.push_back(readOperator());
        }
    }
    tokens.push_back({TokenType::FimArquivo, "", linha, coluna});
    return tokens;
}

} // namespace Kima