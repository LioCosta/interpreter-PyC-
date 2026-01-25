@echo off
echo ========================================
echo    COMPILANDO NOSSO COMPILADOR
echo ========================================
echo.

echo Compilando todos os arquivos...
g++ -std=c++17 -o PYc++.exe main.cpp lexer.cpp parser.cpp semantico.cpp gerador.cpp

if %errorlevel% equ 0 (
    echo.
    echo COMPILAÇÃO CONCLUÍDA COM SUCESSO!
    echo.
    echo Para compilar um programa Python:
    echo   nosso_compilador.exe exemplo.py
    echo.
    echo Isso vai criar:
    echo   1. programa_gerado.c código C
    echo   2. Depois compile com: gcc programa_gerado.c -o programa.exe
    echo   3. Execute: programa.exe
    echo.
    pause
) else (
    echo.
    echo ERRO NA COMPILAÇÃO!
    echo Verifique se tem o GCC instalado.
    echo.
    pause
)