#include <iostream>
#include <fstream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "semantico.h"
#include "gerador.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "Compilador Python-like\n\n";
    
    string arq;
    
    if (argc > 1) arq = argv[1];
    else {
        cout << "Digite o arquivo: ";
        cin >> arq;
    }
    
    try {
        // 1. Ler
        string codigo = Lexer::lerArq(arq);
        
        // 2. Tokenizar
        Lexer lex(codigo);
        vector<Token> tokens = lex.tokenizar();
        
        // 3. Parser
        Parser parser(tokens);
        Bloco* prog = parser.parse();
        
        // 4. Semantico
        TabSim tab;
        Semantico sem(tab);
        sem.check(prog);
        
        // 5. Gerar C
        Gerador gen;
        string c_code = gen.gerar(prog);
        
        // Salvar
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