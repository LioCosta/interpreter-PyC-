#include "parser.hpp"
#include <stdexcept>
#include <iostream>

namespace Kima {

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {
}

Token Parser::peek() const {
    if (atual >= tokens.size()) return tokens.back();
    return tokens[atual];
}

Token Parser::anterior() const {
    if (atual == 0) return tokens[0];
    return tokens[atual - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) atual++;
    return anterior();
}

bool Parser::isAtEnd() const {
    if (atual >= tokens.size()) return true;
    return tokens[atual].tipo == TokenType::FimArquivo;
}

bool Parser::check(TokenType tipo) const {
    if (isAtEnd()) return false;
    return tokens[atual].tipo == tipo;
}

bool Parser::match(TokenType tipo) {
    if (check(tipo)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType tipo, const std::string& mensagem) {
    if (check(tipo)) return advance();
    throw std::runtime_error(mensagem + " na linha " + std::to_string(peek().linha));
}

void Parser::erro(const std::string& mensagem) {
    throw std::runtime_error(mensagem + " na linha " + std::to_string(peek().linha));
}

// ------------- Parsing de Comandos -------------

Cmd Parser::parseDeclaracao() {
    if (match(TokenType::Se)) return parseSe();
    if (match(TokenType::Enquanto)) return parseEnquanto();
    if (match(TokenType::Para)) return parsePara();
    if (match(TokenType::Def)) return parseDef();
    if (match(TokenType::Retorna)) return parseRetorna();
    if (match(TokenType::Escreve)) return parseEscreve();
    if (match(TokenType::Var)) return parseDecl();
    return parseComando();
}

Cmd Parser::parseComando() {
    auto expr = parseExpressao();
    return CmdExpr{std::move(expr)};
}

Cmd Parser::parseSe() {
    consume(TokenType::AbrePar, "Esperado '(' após 'se'");
    auto cond = parseExpressao();
    consume(TokenType::FechaPar, "Esperado ')' após condição");
    auto entao = parseBloco();
    std::vector<Cmd> senao;
    if (match(TokenType::Senao)) {
        senao = parseBloco();
    }
    return CmdIf{std::move(cond), std::move(entao), std::move(senao)};
}

Cmd Parser::parseEnquanto() {
    consume(TokenType::AbrePar, "Esperado '(' após 'enquanto'");
    auto cond = parseExpressao();
    consume(TokenType::FechaPar, "Esperado ')' após condição");
    auto corpo = parseBloco();
    return CmdWhile{std::move(cond), std::move(corpo)};
}

Cmd Parser::parsePara() {
    Token var = consume(TokenType::Ident, "Esperado nome da variável");
    if (!match(TokenType::Ident) || anterior().lexema != "de")
        erro("Esperado 'de' após variável");
    auto inicio = parseExpressao();
    if (!match(TokenType::Ident) || anterior().lexema != "ate")
        erro("Esperado 'ate' após início");
    auto fim = parseExpressao();
    auto corpo = parseBloco();
    return CmdFor{std::string(var.lexema), std::move(inicio), std::move(fim), std::move(corpo)};
}

Cmd Parser::parseDef() {
    Token nome = consume(TokenType::Ident, "Esperado nome da função");
    consume(TokenType::AbrePar, "Esperado '('");
    std::vector<std::string> params;
    std::vector<Tipo> tipos;
    if (!check(TokenType::FechaPar)) {
        do {
            Token param = consume(TokenType::Ident, "Esperado nome do parâmetro");
            params.push_back(std::string(param.lexema));
            if (match(TokenType::DoisPontos)) {
                if (match(TokenType::Int)) tipos.push_back(Tipo::Int);
                else if (match(TokenType::Float)) tipos.push_back(Tipo::Float);
                else if (match(TokenType::String)) tipos.push_back(Tipo::String);
                else if (match(TokenType::Bool)) tipos.push_back(Tipo::Bool);
                else erro("Tipo de parâmetro inválido");
            }
        } while (match(TokenType::Virgula));
    }
    consume(TokenType::FechaPar, "Esperado ')'");
    Tipo ret = Tipo::Void;
    if (match(TokenType::DoisPontos)) {
        if (match(TokenType::Int)) ret = Tipo::Int;
        else if (match(TokenType::Float)) ret = Tipo::Float;
        else if (match(TokenType::String)) ret = Tipo::String;
        else if (match(TokenType::Bool)) ret = Tipo::Bool;
        else erro("Tipo de retorno inválido");
    }
    auto corpo = parseBloco();
    return CmdDef{std::string(nome.lexema), params, tipos, ret, std::move(corpo)};
}

Cmd Parser::parseRetorna() {
    if (check(TokenType::FechaChave) || check(TokenType::FimArquivo)) {
        return CmdReturn{nullptr};
    }
    auto expr = parseExpressao();
    return CmdReturn{std::move(expr)};
}

Cmd Parser::parseEscreve() {
    consume(TokenType::AbrePar, "Esperado '(' após 'escreve'");
    auto expr = parseExpressao();
    consume(TokenType::FechaPar, "Esperado ')'");
    return CmdPrint{std::move(expr)};
}

Cmd Parser::parseDecl() {
    Token nome = consume(TokenType::Ident, "Esperado nome da variável");
    consume(TokenType::DoisPontos, "Esperado ':' após nome");
    Tipo tipo;
    if (match(TokenType::Int)) tipo = Tipo::Int;
    else if (match(TokenType::Float)) tipo = Tipo::Float;
    else if (match(TokenType::String)) tipo = Tipo::String;
    else if (match(TokenType::Bool)) tipo = Tipo::Bool;
    else erro("Tipo inválido");

    std::unique_ptr<Expr> valor;
    if (match(TokenType::Atribuicao)) {
        valor = parseExpressao();
    }
    return CmdDecl{std::string(nome.lexema), tipo, std::move(valor)};
}

// ------------- Blocos -------------

std::vector<Cmd> Parser::parseBloco() {
    consume(TokenType::AbreChave, "Esperado '{'");
    std::vector<Cmd> cmds;
    while (!check(TokenType::FechaChave) && !isAtEnd()) {
        cmds.push_back(parseDeclaracao());
    }
    consume(TokenType::FechaChave, "Esperado '}'");
    return cmds;
}

// ------------- Expressões (com precedência) -------------

std::unique_ptr<Expr> Parser::parseExpressao() {
    return parseAtribuicao();
}

std::unique_ptr<Expr> Parser::parseAtribuicao() {
    auto expr = parseLogicoOu();
    if (match(TokenType::Atribuicao)) {
        auto valor = parseAtribuicao();
        if (auto* var = std::get_if<ExprVar>(expr.get())) {
            return std::make_unique<Expr>(ExprAttrib{var->nome, std::move(valor)});
        } else {
            erro("Atribuição inválida: lado esquerdo deve ser variável");
        }
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseLogicoOu() {
    auto expr = parseLogicoE();
    while (match(TokenType::Ou)) {
        Token op = anterior();
        auto dir = parseLogicoE();
        expr = std::make_unique<Expr>(ExprBinOp{std::string(op.lexema), std::move(expr), std::move(dir)});
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseLogicoE() {
    auto expr = parseIgualdade();
    while (match(TokenType::E)) {
        Token op = anterior();
        auto dir = parseIgualdade();
        expr = std::make_unique<Expr>(ExprBinOp{std::string(op.lexema), std::move(expr), std::move(dir)});
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseIgualdade() {
    auto expr = parseComparacao();
    while (match(TokenType::IgualIgual) || match(TokenType::Diferente)) {
        Token op = anterior();
        auto dir = parseComparacao();
        expr = std::make_unique<Expr>(ExprBinOp{std::string(op.lexema), std::move(expr), std::move(dir)});
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseComparacao() {
    auto expr = parseAdicao();
    while (match(TokenType::Menor) || match(TokenType::Maior) ||
           match(TokenType::MenorIgual) || match(TokenType::MaiorIgual)) {
        Token op = anterior();
        auto dir = parseAdicao();
        expr = std::make_unique<Expr>(ExprBinOp{std::string(op.lexema), std::move(expr), std::move(dir)});
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseAdicao() {
    auto expr = parseMultiplicacao();
    while (match(TokenType::Mais) || match(TokenType::Menos)) {
        Token op = anterior();
        auto dir = parseMultiplicacao();
        expr = std::make_unique<Expr>(ExprBinOp{std::string(op.lexema), std::move(expr), std::move(dir)});
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseMultiplicacao() {
    auto expr = parseUnario();
    while (match(TokenType::Mult) || match(TokenType::Div) || match(TokenType::Mod)) {
        Token op = anterior();
        auto dir = parseUnario();
        expr = std::make_unique<Expr>(ExprBinOp{std::string(op.lexema), std::move(expr), std::move(dir)});
    }
    return expr;
}

std::unique_ptr<Expr> Parser::parseUnario() {
    if (match(TokenType::Menos) || match(TokenType::Nao)) {
        Token op = anterior();
        auto expr = parseUnario();
        return std::make_unique<Expr>(ExprUnOp{std::string(op.lexema), std::move(expr)});
    }
    return parsePrimario();
}

std::unique_ptr<Expr> Parser::parsePrimario() {
    if (match(TokenType::Num)) {
        double val = std::stod(std::string(anterior().lexema));
        return std::make_unique<Expr>(ExprNum{val});
    }
    if (match(TokenType::FloatNum)) {
        double val = std::stod(std::string(anterior().lexema));
        return std::make_unique<Expr>(ExprFloat{val});
    }
    if (match(TokenType::StringLit)) {
        return std::make_unique<Expr>(ExprString{std::string(anterior().lexema)});
    }
    if (match(TokenType::Verdadeiro)) {
        return std::make_unique<Expr>(ExprBool{true});
    }
    if (match(TokenType::Falso)) {
        return std::make_unique<Expr>(ExprBool{false});
    }
    if (match(TokenType::Ident)) {
        std::string nome(anterior().lexema);
        if (match(TokenType::AbrePar)) {
            std::vector<std::unique_ptr<Expr>> args;
            if (!check(TokenType::FechaPar)) {
                do {
                    args.push_back(parseExpressao());
                } while (match(TokenType::Virgula));
            }
            consume(TokenType::FechaPar, "Esperado ')' após argumentos");
            return std::make_unique<Expr>(ExprCall{nome, std::move(args)});
        }
        return std::make_unique<Expr>(ExprVar{nome});
    }
    if (match(TokenType::AbrePar)) {
        auto expr = parseExpressao();
        consume(TokenType::FechaPar, "Esperado ')' após expressão");
        return expr;
    }
    erro("Expressão inválida");
    return nullptr;
}

// ------------- Programa -------------

Programa Parser::parse() {
    Programa prog;
    while (!isAtEnd()) {
        prog.push_back(parseDeclaracao());
    }
    return prog;
}

} // namespace Kima