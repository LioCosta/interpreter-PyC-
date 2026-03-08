#ifndef GERADOR_HPP
#define GERADOR_HPP

#include "ast.hpp"
#include <sstream>
#include <string>

namespace Kima {

class Gerador {
public:
    std::string gerar(Programa& prog);

private:
    std::ostringstream saida;
    int indentacao = 0;

    std::string indent();
    std::string tipoParaC(Tipo t);
    void gerarCabecalhos();
    void gerarFuncoesAuxiliares();

    void gerarCmd(Cmd& cmd);
    void gerarCmdImpl(CmdPrint& c);
    void gerarCmdImpl(CmdIf& c);
    void gerarCmdImpl(CmdWhile& c);
    void gerarCmdImpl(CmdFor& c);
    void gerarCmdImpl(CmdDef& c);
    void gerarCmdImpl(CmdReturn& c);
    void gerarCmdImpl(CmdDecl& c);
    void gerarCmdImpl(CmdExpr& c);

    void gerarExpr(Expr& expr);
    void gerarExprImpl(ExprNum& e);
    void gerarExprImpl(ExprFloat& e);
    void gerarExprImpl(ExprString& e);
    void gerarExprImpl(ExprBool& e);
    void gerarExprImpl(ExprVar& e);
    void gerarExprImpl(ExprBinOp& e);
    void gerarExprImpl(ExprUnOp& e);
    void gerarExprImpl(ExprAttrib& e);
    void gerarExprImpl(ExprCall& e);
};

} // namespace Kima

#endif