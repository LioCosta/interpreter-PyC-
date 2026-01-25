// semantico.h - verifica se tá certo
#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "parser.h"
#include <map>
#include <string>

class TabSim {
private:
    std::map<std::string, int> vars;
    
public:
    void add(const std::string& nome, int val = 0);
    void set(const std::string& nome, int val);
    int get(const std::string& nome) const;
    bool tem(const std::string& nome) const;
    void limpa();
};

class Semantico {
private:
    TabSim& tab;
    
public:
    Semantico(TabSim& t);
    
    void check(Bloco* prog);
    
private:
    void checkCmd(Cmd* cmd);
    void checkExpr(Expr* exp);
    void checkBloco(Bloco* blk);
};

#endif