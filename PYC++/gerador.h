// gerador.h - gera código C
#ifndef GERADOR_H
#define GERADOR_H

#include "parser.h"
#include <string>
#include <sstream>
#include <set>

class Gerador {
private:
    std::ostringstream saida;
    std::set<std::string> vars;
    
    std::string genExpr(Expr* exp);
    std::string genCmd(Cmd* cmd, int nivel);
    std::string genBloco(Bloco* blk, int nivel);
    std::string indent(int nivel);
    
public:
    Gerador();
    std::string gerar(Bloco* prog);
};

#endif