#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <variant>
#include <iostream>

enum class Tipo {
    Inteiro,
    Flutuante,
    String,
    Booleano,
    Indefinido
};

// Um valor genérico para a tabela de símbolos
using Valor = std::variant<int, double, std::string, bool>;

struct Simbolo {
    Tipo tipo;
    Valor valor;
};

class SymbolTable {
private:
    std::unordered_map<std::string, Simbolo> tabela;

public:
    // Adiciona um simbolo
    bool adicionar(const std::string& nome, Tipo tipo, Valor valor = 0);

    // Verifica se existe o simbolo
    bool existe(const std::string& nome);

    // Obtem um simbolo
    Simbolo* obter(const std::string& nome);

    // Atualiza o valor
    bool atualizar(const std::string& nome, Valor valor);
};

#endif

