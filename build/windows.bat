REM Apaga cache antigo do CMake
del /F /Q CMakeCache.txt
rmdir /S /Q CMakeFiles

REM Compila Cmake
cmake .. -G "MinGW Makefiles"

REM Compila
mingw32-make

REM Executa o jogo
game.exe

pause