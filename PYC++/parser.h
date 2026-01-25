// parser.h - monta a árvore
#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>
#include <string>

// Nós da árvore
struct Expr {
    enum Tipo { NUM, VAR, OP, ATT, CALL } tipo;
    
    // Dados dependendo do tipo
    int num_val;                            // para NUM
    std::string var_nome;                   // para VAR
    struct {
        std::string op;
        Expr* left;
        Expr* right;
    } op_data;                              // para OP
    struct {
        std::string nome;
        Expr* valor;
    } att_data;                             // para ATT
    struct {
        std::string func;
        Expr* arg;
    } call_data;                            // para CALL
    
    // Construtor simples
    Expr() : tipo(NUM), num_val(0) {
        op_data.left = nullptr;
        op_data.right = nullptr;
        att_data.valor = nullptr;
        call_data.arg = nullptr;
    }
};

struct Bloco {
    std::vector<struct Cmd*> cmds;
};

struct Cmd {
    enum Tipo { EXP, IF, WHILE } tipo;
    
    // Dados
    Expr* exp;                              // para EXP
    
    struct {
        Expr* cond;
        Bloco* if_blk;
        Bloco* else_blk;
    } if_data;                              // para IF
    
    struct {
        Expr* cond;
        Bloco* corpo;
    } while_data;                           // para WHILE
    
    // Construtor
    Cmd() : tipo(EXP), exp(nullptr) {
        if_data.cond = nullptr;
        if_data.if_blk = nullptr;
        if_data.else_blk = nullptr;
        while_data.cond = nullptr;
        while_data.corpo = nullptr;
    }
};

// Funções para criar nós
Expr* mkNum(int val);
Expr* mkVar(std::string nome);
Expr* mkOp(std::string op, Expr* left, Expr* right);
Expr* mkAtt(std::string nome, Expr* valor);
Expr* mkCall(std::string func, Expr* arg);

// Parser principal
class Parser {
private:
    std::vector<Token> tokens;
    int pos;
    
    Token atual();
    Token prox();
    bool eh(TipoToken tipo);
    bool consome(TipoToken tipo);
    
    Cmd* parseCmd();
    Cmd* parsePrint();
    Cmd* parseIf();
    Cmd* parseWhile();
    Bloco* parseBloco();
    Expr* parseExpr();
    Expr* parseAtt();
    Expr* parseComp();
    Expr* parseSoma();
    Expr* parseMult();
    Expr* parsePrim();
    
public:
    Parser(const std::vector<Token>& t);
    Bloco* parse();
};

#endif