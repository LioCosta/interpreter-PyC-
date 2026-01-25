#include "semantico.h"
#include <stdexcept>
#include <iostream>

using namespace std;

void TabSim::add(const string& nome, int val) {
    if (tem(nome)) throw runtime_error("Variável já existe: " + nome);
    vars[nome] = val;
}

void TabSim::set(const string& nome, int val) {
    if (!tem(nome)) throw runtime_error("Variável não existe: " + nome);
    vars[nome] = val;
}

int TabSim::get(const string& nome) const {
    auto it = vars.find(nome);
    if (it == vars.end()) throw runtime_error("Variável não existe: " + nome);
    return it->second;
}

bool TabSim::tem(const string& nome) const {
    return vars.find(nome) != vars.end();
}

void TabSim::limpa() {
    vars.clear();
}

Semantico::Semantico(TabSim& t) : tab(t) {}

void Semantico::check(Bloco* prog) {
    checkBloco(prog);
}

void Semantico::checkBloco(Bloco* blk) {
    for (Cmd* cmd : blk->cmds) {
        checkCmd(cmd);
    }
}

void Semantico::checkCmd(Cmd* cmd) {
    switch (cmd->tipo) {
        case Cmd::EXP:
            checkExpr(cmd->exp);
            break;
        case Cmd::IF:
            checkExpr(cmd->if_data.cond);
            checkBloco(cmd->if_data.if_blk);
            if (cmd->if_data.else_blk) checkBloco(cmd->if_data.else_blk);
            break;
        case Cmd::WHILE:
            checkExpr(cmd->while_data.cond);
            checkBloco(cmd->while_data.corpo);
            break;
    }
}

void Semantico::checkExpr(Expr* exp) {
    switch (exp->tipo) {
        case Expr::NUM:
            break;
        case Expr::VAR:
            if (!tab.tem(exp->var_nome)) {
                tab.add(exp->var_nome, 0);
            }
            break;
        case Expr::OP:
            checkExpr(exp->op_data.left);
            checkExpr(exp->op_data.right);
            break;
        case Expr::ATT:
            if (!tab.tem(exp->att_data.nome)) {
                tab.add(exp->att_data.nome, 0);
            }
            checkExpr(exp->att_data.valor);
            break;
        case Expr::CALL:
            if (exp->call_data.func == "print") {
                checkExpr(exp->call_data.arg);
            }
            break;
    }
}