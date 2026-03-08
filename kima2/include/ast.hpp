#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <vector>
#include <memory>
#include <variant>

namespace Kima {

// Tipos de dados
enum class Tipo {
    Int, Float, String, Bool, Void
};

// Forward declarations
struct ExprNum;
struct ExprFloat;
struct ExprString;
struct ExprBool;
struct ExprVar;
struct ExprBinOp;
struct ExprUnOp;
struct ExprAttrib;
struct ExprCall;

using Expr = std::variant<
    ExprNum,
    ExprFloat,
    ExprString,
    ExprBool,
    ExprVar,
    ExprBinOp,
    ExprUnOp,
    ExprAttrib,
    ExprCall
>;

// Expressões (definições completas)
struct ExprNum { double valor; };
struct ExprFloat { double valor; };
struct ExprString { std::string valor; };
struct ExprBool { bool valor; };
struct ExprVar { std::string nome; };
struct ExprBinOp {
    std::string op;
    std::unique_ptr<Expr> esquerda;
    std::unique_ptr<Expr> direita;
};
struct ExprUnOp {
    std::string op;
    std::unique_ptr<Expr> expr;
};
struct ExprAttrib {
    std::string nome;
    std::unique_ptr<Expr> valor;
};
struct ExprCall {
    std::string funcao;
    std::vector<std::unique_ptr<Expr>> argumentos;
};

// Forward declarations para comandos
struct CmdPrint;
struct CmdIf;
struct CmdWhile;
struct CmdFor;
struct CmdDef;
struct CmdReturn;
struct CmdDecl;
struct CmdExpr;

using Cmd = std::variant<
    CmdPrint,
    CmdIf,
    CmdWhile,
    CmdFor,
    CmdDef,
    CmdReturn,
    CmdDecl,
    CmdExpr
>;

// Comandos
struct CmdPrint {
    std::unique_ptr<Expr> expr;
};
struct CmdIf {
    std::unique_ptr<Expr> condicao;
    std::vector<Cmd> entao;
    std::vector<Cmd> senao;
};
struct CmdWhile {
    std::unique_ptr<Expr> condicao;
    std::vector<Cmd> corpo;
};
struct CmdFor {
    std::string var;
    std::unique_ptr<Expr> inicio;
    std::unique_ptr<Expr> fim;
    std::vector<Cmd> corpo;
};
struct CmdDef {
    std::string nome;
    std::vector<std::string> parametros;
    std::vector<Tipo> tipos_param;
    Tipo tipo_retorno;
    std::vector<Cmd> corpo;
};
struct CmdReturn {
    std::unique_ptr<Expr> expr;
};
struct CmdDecl {
    std::string nome;
    Tipo tipo;
    std::unique_ptr<Expr> valor_inicial;
};
struct CmdExpr {
    std::unique_ptr<Expr> expr;
};

using Programa = std::vector<Cmd>;

} // namespace Kima

#endif