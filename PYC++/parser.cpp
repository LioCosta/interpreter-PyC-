#include "parser.h"
#include <stdexcept>
#include <iostream>

using namespace std;

// Criar nós
Expr* mkNum(int val) {
    Expr* e = new Expr();
    e->tipo = Expr::NUM;
    e->num_val = val;
    return e;
}

Expr* mkVar(string nome) {
    Expr* e = new Expr();
    e->tipo = Expr::VAR;
    e->var_nome = nome;
    return e;
}

Expr* mkOp(string op, Expr* left, Expr* right) {
    Expr* e = new Expr();
    e->tipo = Expr::OP;
    e->op_data.op = op;
    e->op_data.left = left;
    e->op_data.right = right;
    return e;
}

Expr* mkAtt(string nome, Expr* valor) {
    Expr* e = new Expr();
    e->tipo = Expr::ATT;
    e->att_data.nome = nome;
    e->att_data.valor = valor;
    return e;
}

Expr* mkCall(string func, Expr* arg) {
    Expr* e = new Expr();
    e->tipo = Expr::CALL;
    e->call_data.func = func;
    e->call_data.arg = arg;
    return e;
}

// Parser
Parser::Parser(const vector<Token>& t) {
    tokens = t;
    pos = 0;
}

Token Parser::atual() {
    if (pos < tokens.size()) return tokens[pos];
    return {FIM, "", 0};
}

Token Parser::prox() {
    if (pos < tokens.size()) return tokens[pos++];
    return {FIM, "", 0};
}

bool Parser::eh(TipoToken tipo) {
    return atual().tipo == tipo;
}

bool Parser::consome(TipoToken tipo) {
    if (eh(tipo)) {
        prox();
        return true;
    }
    return false;
}

Bloco* Parser::parse() {
    Bloco* prog = new Bloco();
    
    while (!eh(FIM)) {
        Cmd* cmd = parseCmd();
        if (cmd) prog->cmds.push_back(cmd);
    }
    
    return prog;
}

Cmd* Parser::parseCmd() {
    if (eh(IF)) return parseIf();
    if (eh(WHILE)) return parseWhile();
    if (eh(PRINT)) return parsePrint();
    
    Expr* exp = parseExpr();
    Cmd* cmd = new Cmd();
    cmd->tipo = Cmd::EXP;
    cmd->exp = exp;
    return cmd;
}

Cmd* Parser::parsePrint() {
    prox(); // print
    
    Expr* exp = parseExpr();
    Expr* call = mkCall("print", exp);
    
    Cmd* cmd = new Cmd();
    cmd->tipo = Cmd::EXP;
    cmd->exp = call;
    return cmd;
}

Cmd* Parser::parseIf() {
    prox(); // if
    
    Expr* cond = parseExpr();
    
    if (!consome(DOIS_PT)) throw runtime_error("Faltou :");
    
    Bloco* if_blk = parseBloco();
    Bloco* else_blk = nullptr;
    
    if (eh(ELSE)) {
        prox(); // else
        
        if (!consome(DOIS_PT)) throw runtime_error("Faltou : no else");
        
        else_blk = parseBloco();
    }
    
    Cmd* cmd = new Cmd();
    cmd->tipo = Cmd::IF;
    cmd->if_data.cond = cond;
    cmd->if_data.if_blk = if_blk;
    cmd->if_data.else_blk = else_blk;
    return cmd;
}

Cmd* Parser::parseWhile() {
    prox(); // while
    
    Expr* cond = parseExpr();
    
    if (!consome(DOIS_PT)) throw runtime_error("Faltou :");
    
    Bloco* corpo = parseBloco();
    
    Cmd* cmd = new Cmd();
    cmd->tipo = Cmd::WHILE;
    cmd->while_data.cond = cond;
    cmd->while_data.corpo = corpo;
    return cmd;
}

Bloco* Parser::parseBloco() {
    Bloco* blk = new Bloco();
    
    Cmd* cmd = parseCmd();
    if (cmd) blk->cmds.push_back(cmd);
    
    return blk;
}

Expr* Parser::parseExpr() {
    return parseAtt();
}

Expr* Parser::parseAtt() {
    Expr* left = parseComp();
    
    if (consome(IGUAL)) {
        Expr* right = parseExpr();
        
        if (left->tipo != Expr::VAR) {
            throw runtime_error("Só variável pode receber valor");
        }
        
        return mkAtt(left->var_nome, right);
    }
    
    return left;
}

Expr* Parser::parseComp() {
    Expr* exp = parseSoma();
    
    while (eh(MENOR) || eh(MAIOR) || eh(MENOR_IG) || 
           eh(MAIOR_IG) || eh(IGUAL2) || eh(DIF)) {
        Token op = prox();
        Expr* dir = parseSoma();
        exp = mkOp(op.txt, exp, dir);
    }
    
    return exp;
}

Expr* Parser::parseSoma() {
    Expr* exp = parseMult();
    
    while (eh(MAIS) || eh(MENOS)) {
        Token op = prox();
        Expr* dir = parseMult();
        exp = mkOp(op.txt, exp, dir);
    }
    
    return exp;
}

Expr* Parser::parseMult() {
    Expr* exp = parsePrim();
    
    while (eh(MULT) || eh(DIV)) {
        Token op = prox();
        Expr* dir = parsePrim();
        exp = mkOp(op.txt, exp, dir);
    }
    
    return exp;
}

Expr* Parser::parsePrim() {
    Token t = atual();
    
    if (consome(NUM)) {
        int val = stoi(t.txt);
        return mkNum(val);
    }
    
    if (consome(NOME)) {
        return mkVar(t.txt);
    }
    
    if (consome(INPUT)) {
        return mkCall("input", mkNum(0));
    }
    
    throw runtime_error("Expressão inválida");
}