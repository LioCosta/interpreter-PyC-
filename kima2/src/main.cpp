#include "lexer.hpp"
#include "parser.hpp"
#include "semantico.hpp"
#include "gerador.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace Kima;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <ficheiro.kima>\n";
        return 1;
    }

    std::cout << "A processar ficheiro: " << argv[1] << "\n";

    try {
        std::ifstream file(argv[1]);
        if (!file) {
            std::cerr << "Erro ao abrir ficheiro: " << argv[1] << '\n';
            return 1;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string fonte = buffer.str();
        std::cout << "Fonte lida, tamanho: " << fonte.size() << " caracteres\n";

        Lexer lexer(fonte);
        auto tokens = lexer.tokenizar();
        std::cout << "Tokens gerados: " << tokens.size() << "\n";

        std::cout << "A criar parser...\n";
        Parser parser(tokens);
        std::cout << "A executar parser.parse()...\n";
        auto programa = parser.parse();
        std::cout << "Parse concluído. Número de comandos: " << programa.size() << "\n";

        std::cout << "A criar analisador semântico...\n";
        Semantico semantico;
        std::cout << "A analisar programa...\n";
        semantico.analisar(programa);
        std::cout << "Análise semântica concluída com sucesso\n";

        std::cout << "A criar gerador...\n";
        Gerador gerador;
        std::cout << "A gerar código C++...\n";
        std::string codigoCpp = gerador.gerar(programa);
        std::cout << "Código gerado, tamanho: " << codigoCpp.size() << " caracteres\n";

        std::ofstream out("output.cpp");
        out << codigoCpp;
        out.close();
        std::cout << "Ficheiro output.cpp escrito com sucesso\n";

    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Erro desconhecido (possível crash)\n";
        return 1;
    }

    std::cout << "Programa concluído com sucesso.\n";
    return 0;
}