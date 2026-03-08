#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"
#include "lexer.hpp"
#include <vector>
#include <memory>

namespace Kima {

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);
    Programa parse();

private:
    std::vector<Token> tokens;
    size_t atual = 0;

    Token peek() const;
    Token anterior() const;
    Token advance();
    bool isAtEnd() const;
    bool check(TokenType tipo) const;
    bool match(TokenType tipo);
    Token consume(TokenType tipo, const std::string& mensagem);
    void erro(const std::string& mensagem);

    Cmd parseDeclaracao();
    Cmd parseComando();
    Cmd parseSe();
    Cmd parseEnquanto();
    Cmd parsePara();
    Cmd parseDef();
    Cmd parseRetorna();
    Cmd parseEscreve();
    Cmd parseDecl();
    std::unique_ptr<Expr> parseExpressao();
    std::unique_ptr<Expr> parseAtribuicao();
    std::unique_ptr<Expr> parseLogicoOu();
    std::unique_ptr<Expr> parseLogicoE();
    std::unique_ptr<Expr> parseIgualdade();
    std::unique_ptr<Expr> parseComparacao();
    std::unique_ptr<Expr> parseAdicao();
    std::unique_ptr<Expr> parseMultiplicacao();
    std::unique_ptr<Expr> parseUnario();
    std::unique_ptr<Expr> parsePrimario();
    std::vector<Cmd> parseBloco();
};

} // namespace Kima

#endif