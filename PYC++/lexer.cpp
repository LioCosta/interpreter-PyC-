#include "lexer.h"
#include <fstream>
#include <cctype>
#include <iostream>

using namespace std;

Lexer::Lexer(const std::string& txt) {
    codigo = txt;
    pos = 0;
    linha = 1;
}

string Lexer::lerArq(const string& nome) {
    ifstream arq(nome);
    if (!arq) throw runtime_error("Erro ao abrir: " + nome);
    
    string tudo;
    string lin;
    
    while (getline(arq, lin)) {
        tudo += lin + "\n";
    }
    
    return tudo;
}

char Lexer::prox() {
    if (pos >= codigo.length()) return '\0';
    return codigo[pos];
}

char Lexer::avancar() {
    if (pos >= codigo.length()) return '\0';
    return codigo[pos++];
}

vector<Token> Lexer::tokenizar() {
    vector<Token> tokens;
    
    while (pos < codigo.length()) {
        char c = prox();
        
        if (c == ' ' || c == '\t') {
            avancar();
            continue;
        }
        
        if (c == '\n') {
            avancar();
            linha++;
            continue;
        }
        
        // Números
        if (isdigit(c)) {
            string num;
            while (isdigit(prox())) {
                num += avancar();
            }
            tokens.push_back({NUM, num, linha});
            continue;
        }
        
        // Nomes
        if (isalpha(c) || c == '_') {
            string nome;
            while (isalnum(prox()) || prox() == '_') {
                nome += avancar();
            }
            
            if (nome == "print") tokens.push_back({PRINT, nome, linha});
            else if (nome == "if") tokens.push_back({IF, nome, linha});
            else if (nome == "else") tokens.push_back({ELSE, nome, linha});
            else if (nome == "while") tokens.push_back({WHILE, nome, linha});
            else if (nome == "input") tokens.push_back({INPUT, nome, linha});
            else tokens.push_back({NOME, nome, linha});
            continue;
        }
        
        // Operadores
        switch (c) {
            case '+': avancar(); tokens.push_back({MAIS, "+", linha}); break;
            case '-': avancar(); tokens.push_back({MENOS, "-", linha}); break;
            case '*': avancar(); tokens.push_back({MULT, "*", linha}); break;
            case '/': avancar(); tokens.push_back({DIV, "/", linha}); break;
            case '=':
                avancar();
                if (prox() == '=') {
                    avancar();
                    tokens.push_back({IGUAL2, "==", linha});
                } else {
                    tokens.push_back({IGUAL, "=", linha});
                }
                break;
            case '!':
                avancar();
                if (prox() == '=') {
                    avancar();
                    tokens.push_back({DIF, "!=", linha});
                }
                break;
            case '<':
                avancar();
                if (prox() == '=') {
                    avancar();
                    tokens.push_back({MENOR_IG, "<=", linha});
                } else {
                    tokens.push_back({MENOR, "<", linha});
                }
                break;
            case '>':
                avancar();
                if (prox() == '=') {
                    avancar();
                    tokens.push_back({MAIOR_IG, ">=", linha});
                } else {
                    tokens.push_back({MAIOR, ">", linha});
                }
                break;
            case ':': avancar(); tokens.push_back({DOIS_PT, ":", linha}); break;
            default:
                avancar();
                tokens.push_back({ERRO, string(1, c), linha});
                break;
        }
    }
    
    tokens.push_back({FIM, "", linha});
    return tokens;
}