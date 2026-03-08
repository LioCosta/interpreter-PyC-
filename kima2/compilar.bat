@echo off
echo ============================================
echo    Compilador KIMA - Dialeto TLP 2026
echo    Grupo: Laerte, Edivaldo, Jorge
echo ============================================
echo.
echo A compilar o compilador KIMA...

g++ -std=c++17 -Iinclude -c src/lexer.cpp     -o src/lexer.o
g++ -std=c++17 -Iinclude -c src/parser.cpp    -o src/parser.o
g++ -std=c++17 -Iinclude -c src/semantico.cpp -o src/semantico.o
g++ -std=c++17 -Iinclude -c src/gerador.cpp   -o src/gerador.o
g++ -std=c++17 -Iinclude -c src/main.cpp      -o src/main.o

g++ -std=c++17 -o kima.exe src/lexer.o src/parser.o src/semantico.o src/gerador.o src/main.o

echo.
echo Compilacao concluida! Executavel: kima.exe
echo.
echo Para usar:
echo   kima.exe exemplos\exemplo.kima
echo   g++ -std=c++17 output.cpp -o programa.exe
echo   programa.exe
echo.
pause
