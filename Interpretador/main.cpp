#include <iostream>
#include "lexer.h"
#include "parser.h"

int main() {
    std::string code;

    std::cout << "Digite o codigo:\n";
    std::getline(std::cin, code);

    Lexer lexer(code);
    Parser parser(lexer);

    AST* tree = parser.parse();

    std::cout << "AST criada com sucesso!\n";

    return 0;
}

