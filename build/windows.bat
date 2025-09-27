REM Apaga cache antigo do CMake
del /F /Q CMakeCache.txt
rmdir /S /Q CMakeFiles

<<<<<<< HEAD
REM Compila Cmake
cmake .. -G "MinGW Makefiles"

=======
>>>>>>> 3bacb7a (adicionado as dlls para windows)
REM Compila
mingw32-make

REM Executa o jogo
game.exe

pause