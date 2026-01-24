#include "symbol_table.h"

bool SymbolTable::adicionar(const std::string& nome, Tipo tipo, Valor valor) {
    if (tabela.find(nome) != tabela.end()) return false; // já existe
    tabela[nome] = {tipo, valor};
    return true;
}

bool SymbolTable::existe(const std::string& nome) {
    return tabela.find(nome) != tabela.end();
}

Simbolo* SymbolTable::obter(const std::string& nome) {
    auto it = tabela.find(nome);
    if (it != tabela.end()) return &it->second;
    return nullptr;
}

bool SymbolTable::atualizar(const std::string& nome, Valor valor) {
    auto it = tabela.find(nome);
    if (it == tabela.end()) return false;
    it->second.valor = valor;
    return true;
}

