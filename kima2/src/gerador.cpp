#include "gerador.hpp"
#include <iostream>

namespace Kima {

std::string Gerador::indent() {
    return std::string(indentacao * 4, ' ');
}

std::string Gerador::tipoParaC(Tipo t) {
    switch (t) {
        case Tipo::Int: return "int";
        case Tipo::Float: return "double";
        case Tipo::String: return "std::string";
        case Tipo::Bool: return "bool";
        case Tipo::Void: return "void";
    }
    return "int";
}

void Gerador::gerarCabecalhos() {
    saida << "#include <iostream>\n";
    saida << "#include <string>\n";
    saida << "#include <cmath>\n";
    saida << "using namespace std;\n\n";
}

void Gerador::gerarFuncoesAuxiliares() {
    saida << "string __kima_read() {\n";
    saida << "    string s;\n";
    saida << "    getline(cin, s);\n";
    saida << "    return s;\n";
    saida << "}\n\n";

    saida << "int __kima_toint(string s) { return stoi(s); }\n";
    saida << "double __kima_tofloat(string s) { return stod(s); }\n";
    saida << "string __kima_tostring(int x) { return to_string(x); }\n";
    saida << "string __kima_tostring(double x) { return to_string(x); }\n";
    saida << "string __kima_tostring(bool x) { return x ? \"verdadeiro\" : \"falso\"; }\n\n";
}

std::string Gerador::gerar(Programa& prog) {
    saida.str("");
    gerarCabecalhos();
    gerarFuncoesAuxiliares();

    // Gerar funções definidas pelo utilizador
    for (auto& cmd : prog) {
        if (auto* def = std::get_if<CmdDef>(&cmd)) {
            saida << tipoParaC(def->tipo_retorno) << " " << def->nome << "(";
            for (size_t i = 0; i < def->parametros.size(); ++i) {
                if (i > 0) saida << ", ";
                Tipo tipo = (i < def->tipos_param.size()) ? def->tipos_param[i] : Tipo::Int;
                saida << tipoParaC(tipo) << " " << def->parametros[i];
            }
            saida << ") {\n";
            indentacao = 1;
            for (auto& c : def->corpo) gerarCmd(c);
            indentacao = 0;
            saida << "}\n\n";
        }
    }

    // Gerar main
    saida << "int main() {\n";
    indentacao = 1;
    for (auto& cmd : prog) {
        if (std::holds_alternative<CmdDef>(cmd)) continue;
        gerarCmd(cmd);
    }
    indentacao = 0;
    saida << "    return 0;\n";
    saida << "}\n";

    return saida.str();
}

void Gerador::gerarCmd(Cmd& cmd) {
    std::visit([this](auto& c) { this->gerarCmdImpl(c); }, cmd);
}

void Gerador::gerarCmdImpl(CmdPrint& c) {
    saida << indent() << "cout << ";
    gerarExpr(*c.expr);
    saida << " << endl;\n";
}

void Gerador::gerarCmdImpl(CmdIf& c) {
    saida << indent() << "if (";
    gerarExpr(*c.condicao);
    saida << ") {\n";
    indentacao++;
    for (auto& cmd : c.entao) gerarCmd(cmd);
    indentacao--;
    saida << indent() << "}";
    if (!c.senao.empty()) {
        saida << " else {\n";
        indentacao++;
        for (auto& cmd : c.senao) gerarCmd(cmd);
        indentacao--;
        saida << indent() << "}";
    }
    saida << "\n";
}

void Gerador::gerarCmdImpl(CmdWhile& c) {
    saida << indent() << "while (";
    gerarExpr(*c.condicao);
    saida << ") {\n";
    indentacao++;
    for (auto& cmd : c.corpo) gerarCmd(cmd);
    indentacao--;
    saida << indent() << "}\n";
}

void Gerador::gerarCmdImpl(CmdFor& c) {
    saida << indent() << "for (int " << c.var << " = ";
    gerarExpr(*c.inicio);
    saida << "; " << c.var << " < ";
    gerarExpr(*c.fim);
    saida << "; ++" << c.var << ") {\n";
    indentacao++;
    for (auto& cmd : c.corpo) gerarCmd(cmd);
    indentacao--;
    saida << indent() << "}\n";
}

void Gerador::gerarCmdImpl(CmdDef& c) {
    // já gerado antes
}

void Gerador::gerarCmdImpl(CmdReturn& c) {
    saida << indent() << "return";
    if (c.expr) {
        saida << " ";
        gerarExpr(*c.expr);
    }
    saida << ";\n";
}

void Gerador::gerarCmdImpl(CmdDecl& c) {
    saida << indent() << tipoParaC(c.tipo) << " " << c.nome;
    if (c.valor_inicial) {
        saida << " = ";
        gerarExpr(*c.valor_inicial);
    } else {
        if (c.tipo == Tipo::Int) saida << " = 0";
        else if (c.tipo == Tipo::Float) saida << " = 0.0";
        else if (c.tipo == Tipo::String) saida << " = \"\"";
        else if (c.tipo == Tipo::Bool) saida << " = false";
    }
    saida << ";\n";
}

void Gerador::gerarCmdImpl(CmdExpr& c) {
    gerarExpr(*c.expr);
    saida << ";\n";
}

void Gerador::gerarExpr(Expr& expr) {
    std::visit([this](auto& e) { this->gerarExprImpl(e); }, expr);
}

void Gerador::gerarExprImpl(ExprNum& e) {
    saida << e.valor;
}

void Gerador::gerarExprImpl(ExprFloat& e) {
    saida << e.valor;
}

void Gerador::gerarExprImpl(ExprString& e) {
    saida << "\"" << e.valor << "\"";
}

void Gerador::gerarExprImpl(ExprBool& e) {
    saida << (e.valor ? "true" : "false");
}

void Gerador::gerarExprImpl(ExprVar& e) {
    saida << e.nome;
}

void Gerador::gerarExprImpl(ExprBinOp& e) {
    saida << "(";
    gerarExpr(*e.esquerda);
    std::string op = e.op;
    if (op == "e") op = "&&";
    else if (op == "ou") op = "||";
    else if (op == "nao") op = "!"; // não usado aqui
    saida << " " << op << " ";
    gerarExpr(*e.direita);
    saida << ")";
}

void Gerador::gerarExprImpl(ExprUnOp& e) {
    std::string op = e.op;
    if (op == "nao") op = "!";
    saida << op;
    gerarExpr(*e.expr);
}

void Gerador::gerarExprImpl(ExprAttrib& e) {
    saida << e.nome << " = ";
    gerarExpr(*e.valor);
}

void Gerador::gerarExprImpl(ExprCall& e) {
    if (e.funcao == "escreve") {
        saida << "cout << ";
        gerarExpr(*e.argumentos[0]);
        return;
    }
    if (e.funcao == "le") {
        saida << "__kima_read()";
        return;
    }
    if (e.funcao == "inteiro") {
        saida << "__kima_toint(";
        gerarExpr(*e.argumentos[0]);
        saida << ")";
        return;
    }
    if (e.funcao == "decimal") {
        saida << "__kima_tofloat(";
        gerarExpr(*e.argumentos[0]);
        saida << ")";
        return;
    }
    if (e.funcao == "texto") {
        saida << "__kima_tostring(";
        gerarExpr(*e.argumentos[0]);
        saida << ")";
        return;
    }
    if (e.funcao == "abs") {
        saida << "abs(";
        gerarExpr(*e.argumentos[0]);
        saida << ")";
        return;
    }
    if (e.funcao == "max" || e.funcao == "min") {
        saida << e.funcao << "(";
        for (size_t i = 0; i < e.argumentos.size(); ++i) {
            if (i > 0) saida << ", ";
            gerarExpr(*e.argumentos[i]);
        }
        saida << ")";
        return;
    }
    // Função definida pelo utilizador
    saida << e.funcao << "(";
    for (size_t i = 0; i < e.argumentos.size(); ++i) {
        if (i > 0) saida << ", ";
        gerarExpr(*e.argumentos[i]);
    }
    saida << ")";
}

} // namespace Kima