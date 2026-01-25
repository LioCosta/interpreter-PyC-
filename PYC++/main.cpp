#include <iostream>
#include <fstream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "semantico.h"
#include "gerador.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Compilador PyC++\n\n";
    
    string arq;
    
    if (argc > 1) arq = argv[1];
    else {
        cout << "Digite o arquivo: ";
        cin >> arq;
    }
    
    try {
        
        string codigo = Lexer::lerArq(arq);
        
        
        Lexer lex(codigo);
        vector<Token> tokens = lex.tokenizar();
        
        Parser parser(tokens);
        Bloco* prog = parser.parse();
        TabSim tab;
        Semantico sem(tab);
        sem.check(prog);

        Gerador gen;
        string c_code = gen.gerar(prog);

        ofstream out("output.c");
        out << c_code;
        out.close();
        
        cout << "Feito! output.c criado\n";
        
    } catch (const exception& e) {
        cout << "Erro: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}