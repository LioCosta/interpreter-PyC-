#include "ast.h"
#include "symbol_table.h"
#include <iostream>

class SemanticAnalyzer {
private:
    SymbolTable symTable;

public:
    void analisar(ASTNode* node) {
        if (!node) return;

        switch (node->tipo) {
            case ASTNodeType::VariavelDecl: {
                std::string nome = node->nome;
                Tipo tipo = node->tipoVar;
                if (!symTable.adicionar(nome, tipo)) {
                    std::cerr << "Erro: Variável '" << nome << "' já declarada.\n";
                }
                break;
            }
            case ASTNodeType::Atribuicao: {
                std::string nome = node->nome;
                if (!symTable.existe(nome)) {
                    std::cerr << "Erro: Variável '" << nome << "' não declarada.\n";
                    break;
                }
                // Aqui poderia verificar tipo do valor
                break;
            }
            case ASTNodeType::Expressao: {
                // Validar tipos da expressão
                break;
            }
            default:
                break;
        }

        // Analisa filhos da AST recursivamente
        for (ASTNode* child : node->filhos) {
            analisar(child);
        }
    }
};

