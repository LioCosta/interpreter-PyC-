#ifndef SEMANTICO_HPP
#define SEMANTICO_HPP

#include "ast.hpp"
#include <unordered_map>
#include <vector>
#include <string>

namespace Kima {

struct Simbolo {
    Tipo tipo;
};

struct FuncaoInfo {
    std::vector<Tipo> param_tipos;
    Tipo retorno;
};

class TabelaSimbolos {
public:
    void entrarEscopo();
    void sairEscopo();
    void adicionar(const std::string& nome, Tipo tipo);
    Simbolo* obter(const std::string& nome);
    bool existeNoEscopoAtual(const std::string& nome);

private:
    std::vector<std::unordered_map<std::string, Simbolo>> pilha;
};

class Semantico {
public:
    Semantico();
    void analisar(Programa& prog);

private:
    TabelaSimbolos tabela;
    std::unordered_map<std::string, FuncaoInfo> funcoes;

    void analisarCmd(Cmd& cmd);
    void analisarCmdImpl(CmdPrint& c);
    void analisarCmdImpl(CmdIf& c);
    void analisarCmdImpl(CmdWhile& c);
    void analisarCmdImpl(CmdFor& c);
    void analisarCmdImpl(CmdDef& c);
    void analisarCmdImpl(CmdReturn& c);
    void analisarCmdImpl(CmdDecl& c);
    void analisarCmdImpl(CmdExpr& c);

    Tipo analisarExpr(Expr& expr);
    Tipo analisarExprImpl(ExprNum& e);
    Tipo analisarExprImpl(ExprFloat& e);
    Tipo analisarExprImpl(ExprString& e);
    Tipo analisarExprImpl(ExprBool& e);
    Tipo analisarExprImpl(ExprVar& e);
    Tipo analisarExprImpl(ExprBinOp& e);
    Tipo analisarExprImpl(ExprUnOp& e);
    Tipo analisarExprImpl(ExprAttrib& e);
    Tipo analisarExprImpl(ExprCall& e);

    void analisarBloco(std::vector<Cmd>& bloco);
    void verificarChamada(ExprCall& call);
};

} // namespace Kima

#endif