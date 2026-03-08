# Compilador KIMA — Dialeto da Linguagem TLP

**Grupo:** Laerte, Edivaldo, Jorge  
**Disciplina:** TLP — 11.ª Classe  
**Professor:** Giovani Vita  
**Data:** Março 2026  

---

## O que é o KIMA?

KIMA é um dialeto de linguagem de programação com palavras-chave em Português/Kimbundu.  
O compilador traduz código `.kima` para C++ e gera um executável.

---

## Palavras-chave da linguagem

| KIMA         | Significado             |
|-------------|-------------------------|
| `inicio`     | Ponto de entrada        |
| `var`        | Declaração de variável  |
| `def`        | Definição de função     |
| `retorna`    | Retorno de função       |
| `se` / `senao` | Condicional           |
| `enquanto`   | Ciclo while             |
| `para ... de ... ate` | Ciclo for      |
| `escreve`    | Imprimir no ecrã        |
| `le`         | Ler da entrada          |
| `inteiro`    | Tipo int                |
| `decimal`    | Tipo double             |
| `texto`      | Tipo string             |
| `logico`     | Tipo bool               |
| `verdadeiro` / `falso` | Valores booleanos |
| `e` / `ou` / `nao` | Operadores lógicos |

---

## Estrutura do Projeto

```
kima/
├── include/          ← Cabeçalhos (.hpp)
│   ├── ast.hpp       ← Árvore Sintática Abstrata
│   ├── lexer.hpp
│   ├── parser.hpp
│   ├── semantico.hpp
│   └── gerador.hpp
├── src/              ← Implementações (.cpp)
│   ├── lexer.cpp     ← Analisador Léxico
│   ├── parser.cpp    ← Analisador Sintático
│   ├── semantico.cpp ← Analisador Semântico
│   ├── gerador.cpp   ← Gerador de Código C++
│   └── main.cpp      ← Ponto de entrada
├── exemplos/
│   └── exemplo.kima  ← Exemplo de programa KIMA
└── compilar.bat      ← Script de compilação (Windows)
```

---

## Como Compilar e Usar (Windows)

### 1. Compilar o compilador KIMA
```
compilar.bat
```

### 2. Compilar um programa .kima
```
kima.exe exemplos\exemplo.kima
```
→ Gera `output.cpp`

### 3. Compilar o C++ gerado
```
g++ -std=c++17 output.cpp -o programa.exe
```

### 4. Executar
```
programa.exe
```

---

## Exemplo de Programa KIMA

```
def soma(a: inteiro, b: inteiro): inteiro {
    retorna a + b
}

def factorial(n: inteiro): inteiro {
    se (n <= 1) {
        retorna 1
    } senao {
        retorna n * factorial(n - 1)
    }
}

var x: inteiro = 5
escreve(soma(x, 3))
escreve(factorial(x))

para i de 1 ate 5 {
    escreve(i)
}
```

### Saída esperada:
```
8
120
1
2
3
4
```

---

## Arquitectura do Compilador

O pipeline de compilação tem 4 fases:

1. **Lexer** — converte o texto em tokens
2. **Parser** — constrói a Árvore Sintática Abstrata (AST) usando `std::variant`
3. **Analisador Semântico** — verifica tipos e âmbitos com tabela de símbolos
4. **Gerador de Código** — produz código C++ válido
