#include "gerador.h"
#include <iostream>

using namespace std;

Gerador::Gerador() {}

string Gerador::indent(int nivel) {
    return string(nivel * 4, ' ');
}

string Gerador::gerar(Bloco* prog) {
    saida.str("");
    vars.clear();
    
    saida << "#include <stdio.h>\n\n";
    saida << "int main() {\n";
    
    saida << genBloco(prog, 1);
    
    saida << "    return 0;\n";
    saida << "}\n";
    
    return saida.str();
}

string Gerador::genBloco(Bloco* blk, int nivel) {
    string res;
    
    for (Cmd* cmd : blk->cmds) {
        res += genCmd(cmd, nivel);
    }
    
    return res;
}

string Gerador::genCmd(Cmd* cmd, int nivel) {
    string ind = indent(nivel);
    string res;
    
    switch (cmd->tipo) {
        case Cmd::EXP: {
            string exp = genExpr(cmd->exp);
            if (!exp.empty()) res = ind + exp + ";\n";
            break;
        }
        case Cmd::IF: {
            string cond = genExpr(cmd->if_data.cond);
            
            res = ind + "if (" + cond + ") {\n";
            res += genBloco(cmd->if_data.if_blk, nivel + 1);
            res += ind + "}";
            
            if (cmd->if_data.else_blk) {
                res += " else {\n";
                res += genBloco(cmd->if_data.else_blk, nivel + 1);
                res += ind + "}";
            }
            
            res += "\n";
            break;
        }
        case Cmd::WHILE: {
            string cond = genExpr(cmd->while_data.cond);
            
            res = ind + "while (" + cond + ") {\n";
            res += genBloco(cmd->while_data.corpo, nivel + 1);
            res += ind + "}\n";
            break;
        }
    }
    
    return res;
}

string Gerador::genExpr(Expr* exp) {
    switch (exp->tipo) {
        case Expr::NUM:
            return to_string(exp->num_val);
        case Expr::VAR:
            return exp->var_nome;
        case Expr::OP: {
            string left = genExpr(exp->op_data.left);
            string right = genExpr(exp->op_data.right);
            return "(" + left + " " + exp->op_data.op + " " + right + ")";
        }
        case Expr::ATT: {
            string nome = exp->att_data.nome;
            string val = genExpr(exp->att_data.valor);
            
            if (vars.find(nome) == vars.end()) {
                vars.insert(nome);
                return "int " + nome + " = " + val;
            } else {
                return nome + " = " + val;
            }
        }
        case Expr::CALL: {
            if (exp->call_data.func == "print") {
                string arg = genExpr(exp->call_data.arg);
                return "printf(\"%d\\n\", " + arg + ")";
            } else if (exp->call_data.func == "input") {
                return "0";
            }
            return "";
        }
    }
    
    return "";
}