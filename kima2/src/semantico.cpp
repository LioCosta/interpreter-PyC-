#include "semantico.hpp"
#include <stdexcept>

namespace Kima {

// ---------------- TabelaSimbolos ----------------
void TabelaSimbolos::entrarEscopo() {
    pilha.emplace_back();
}

void TabelaSimbolos::sairEscopo() {
    pilha.pop_back();
}

void TabelaSimbolos::adicionar(const std::string& nome, Tipo tipo) {
    if (pilha.empty()) pilha.emplace_back();
    if (pilha.back().count(nome))
        throw std::runtime_error("Variável já declarada neste escopo: " + nome);
    pilha.back()[nome] = {tipo};
}

Simbolo* TabelaSimbolos::obter(const std::string& nome) {
    for (auto it = pilha.rbegin(); it != pilha.rend(); ++it) {
        auto found = it->find(nome);
        if (found != it->end()) return &found->second;
    }
    return nullptr;
}

bool TabelaSimbolos::existeNoEscopoAtual(const std::string& nome) {
    if (pilha.empty()) return false;
    return pilha.back().count(nome) > 0;
}

// ---------------- Semantico ----------------
Semantico::Semantico() {
    tabela.entrarEscopo(); // escopo global
}

void Semantico::analisar(Programa& prog) {
    // Registrar funções primeiro
    for (auto& cmd : prog) {
        if (auto* def = std::get_if<CmdDef>(&cmd)) {
            if (funcoes.count(def->nome))
                throw std::runtime_error("Função já definida: " + def->nome);
            funcoes[def->nome] = {def->tipos_param, def->tipo_retorno};
        }
    }
    for (auto& cmd : prog) {
        analisarCmd(cmd);
    }
}

void Semantico::analisarCmd(Cmd& cmd) {
    std::visit([this](auto& c) { this->analisarCmdImpl(c); }, cmd);
}

void Semantico::analisarCmdImpl(CmdPrint& c) {
    analisarExpr(*c.expr);
}

void Semantico::analisarCmdImpl(CmdIf& c) {
    Tipo t = analisarExpr(*c.condicao);
    if (t != Tipo::Bool)
        throw std::runtime_error("Condição do 'se' deve ser booleana");
    analisarBloco(c.entao);
    analisarBloco(c.senao);
}

void Semantico::analisarCmdImpl(CmdWhile& c) {
    Tipo t = analisarExpr(*c.condicao);
    if (t != Tipo::Bool)
        throw std::runtime_error("Condição do 'enquanto' deve ser booleana");
    analisarBloco(c.corpo);
}

void Semantico::analisarCmdImpl(CmdFor& c) {
    // A variável do for deve ser declarada antes? Vamos permitir que seja uma nova variável local.
    // Para simplificar, assumimos que a variável é inteira e local ao bloco.
    // Mas precisamos de a adicionar à tabela dentro do escopo do bloco.
    // Como o corpo ainda não foi analisado, faremos a análise do corpo depois de adicionar a variável.
    tabela.entrarEscopo();
    tabela.adicionar(c.var, Tipo::Int);
    Tipo tInicio = analisarExpr(*c.inicio);
    Tipo tFim = analisarExpr(*c.fim);
    if (tInicio != Tipo::Int || tFim != Tipo::Int)
        throw std::runtime_error("Limites do 'para' devem ser inteiros");
    analisarBloco(c.corpo);
    tabela.sairEscopo();
}

void Semantico::analisarCmdImpl(CmdDef& c) {
    tabela.entrarEscopo();
    for (size_t i = 0; i < c.parametros.size(); ++i) {
        Tipo tipo = (i < c.tipos_param.size()) ? c.tipos_param[i] : Tipo::Int;
        tabela.adicionar(c.parametros[i], tipo);
    }
    analisarBloco(c.corpo);
    tabela.sairEscopo();
}

void Semantico::analisarCmdImpl(CmdReturn& c) {
    if (c.expr) {
        analisarExpr(*c.expr);
    }
}

void Semantico::analisarCmdImpl(CmdDecl& c) {
    if (tabela.existeNoEscopoAtual(c.nome))
        throw std::runtime_error("Variável já declarada: " + c.nome);
    if (c.valor_inicial) {
        Tipo t = analisarExpr(*c.valor_inicial);
        if (t != c.tipo)
            throw std::runtime_error("Tipo do valor inicial não corresponde à declaração");
    }
    tabela.adicionar(c.nome, c.tipo);
}

void Semantico::analisarCmdImpl(CmdExpr& c) {
    analisarExpr(*c.expr);
}

void Semantico::analisarBloco(std::vector<Cmd>& bloco) {
    for (auto& cmd : bloco) {
        analisarCmd(cmd);
    }
}

Tipo Semantico::analisarExpr(Expr& expr) {
    return std::visit([this](auto& e) { return this->analisarExprImpl(e); }, expr);
}

Tipo Semantico::analisarExprImpl(ExprNum& e) {
    return Tipo::Int;
}

Tipo Semantico::analisarExprImpl(ExprFloat& e) {
    return Tipo::Float;
}

Tipo Semantico::analisarExprImpl(ExprString& e) {
    return Tipo::String;
}

Tipo Semantico::analisarExprImpl(ExprBool& e) {
    return Tipo::Bool;
}

Tipo Semantico::analisarExprImpl(ExprVar& e) {
    auto* sim = tabela.obter(e.nome);
    if (!sim) throw std::runtime_error("Variável não declarada: " + e.nome);
    return sim->tipo;
}

Tipo Semantico::analisarExprImpl(ExprBinOp& e) {
    Tipo esq = analisarExpr(*e.esquerda);
    Tipo dir = analisarExpr(*e.direita);
    std::string op = e.op;

    if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
        if (esq != Tipo::Int && esq != Tipo::Float)
            throw std::runtime_error("Operando esquerdo não numérico");
        if (dir != Tipo::Int && dir != Tipo::Float)
            throw std::runtime_error("Operando direito não numérico");
        if (esq == Tipo::Float || dir == Tipo::Float)
            return Tipo::Float;
        return Tipo::Int;
    }
    if (op == "==" || op == "!=" || op == "<" || op == ">" || op == "<=" || op == ">=") {
        if (esq != dir)
            throw std::runtime_error("Tipos incompatíveis na comparação");
        return Tipo::Bool;
    }
    if (op == "e" || op == "ou") {
        if (esq != Tipo::Bool || dir != Tipo::Bool)
            throw std::runtime_error("Operadores lógicos exigem booleanos");
        return Tipo::Bool;
    }
    throw std::runtime_error("Operador desconhecido: " + op);
}

Tipo Semantico::analisarExprImpl(ExprUnOp& e) {
    Tipo t = analisarExpr(*e.expr);
    if (e.op == "-") {
        if (t != Tipo::Int && t != Tipo::Float)
            throw std::runtime_error("Operador unário '-' exige número");
        return t;
    } else if (e.op == "nao") {
        if (t != Tipo::Bool)
            throw std::runtime_error("Operador 'nao' exige booleano");
        return Tipo::Bool;
    }
    throw std::runtime_error("Operador unário desconhecido: " + e.op);
}

Tipo Semantico::analisarExprImpl(ExprAttrib& e) {
    auto* sim = tabela.obter(e.nome);
    if (!sim) throw std::runtime_error("Variável não declarada: " + e.nome);
    Tipo t = analisarExpr(*e.valor);
    if (t != sim->tipo)
        throw std::runtime_error("Tipo incompatível na atribuição");
    return t;
}

Tipo Semantico::analisarExprImpl(ExprCall& e) {
    verificarChamada(e);
    // Retornar tipo da função
    auto it = funcoes.find(e.funcao);
    if (it != funcoes.end()) return it->second.retorno;
    // Funções built-in
    if (e.funcao == "escreve" || e.funcao == "le") return Tipo::Void;
    if (e.funcao == "inteiro") return Tipo::Int;
    if (e.funcao == "decimal") return Tipo::Float;
    if (e.funcao == "texto") return Tipo::String;
    if (e.funcao == "abs" || e.funcao == "max" || e.funcao == "min") {
        // Para simplificar, assumimos que retornam o mesmo tipo do primeiro argumento
        if (e.argumentos.empty()) throw std::runtime_error(e.funcao + " espera argumentos");
        return analisarExpr(*e.argumentos[0]);
    }
    throw std::runtime_error("Função não definida: " + e.funcao);
}

void Semantico::verificarChamada(ExprCall& call) {
    if (call.funcao == "escreve") {
        if (call.argumentos.size() != 1)
            throw std::runtime_error("escreve espera 1 argumento");
        analisarExpr(*call.argumentos[0]);
        return;
    }
    if (call.funcao == "le") {
        if (call.argumentos.size() != 0)
            throw std::runtime_error("le não espera argumentos");
        return;
    }
    if (call.funcao == "inteiro" || call.funcao == "decimal" || call.funcao == "texto") {
        if (call.argumentos.size() != 1)
            throw std::runtime_error(call.funcao + " espera 1 argumento");
        analisarExpr(*call.argumentos[0]);
        return;
    }
    if (call.funcao == "abs" || call.funcao == "max" || call.funcao == "min") {
        if (call.argumentos.empty())
            throw std::runtime_error(call.funcao + " espera pelo menos 1 argumento");
        for (auto& arg : call.argumentos) analisarExpr(*arg);
        return;
    }
    // Função definida pelo utilizador
    auto it = funcoes.find(call.funcao);
    if (it == funcoes.end())
        throw std::runtime_error("Função não definida: " + call.funcao);
    if (it->second.param_tipos.size() != call.argumentos.size())
        throw std::runtime_error("Número de argumentos incorreto para " + call.funcao);
    for (size_t i = 0; i < call.argumentos.size(); ++i) {
        Tipo t = analisarExpr(*call.argumentos[i]);
        if (t != it->second.param_tipos[i])
            throw std::runtime_error("Tipo do argumento " + std::to_string(i+1) + " incorreto");
    }
}

} // namespace Kima