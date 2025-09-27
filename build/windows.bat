REM Apaga cache antigo do CMake
del /F /Q CMakeCache.txt
rmdir /S /Q CMakeFiles

<<<<<<< HEAD
<<<<<<< HEAD
REM Compila Cmake
cmake .. -G "MinGW Makefiles"

=======
>>>>>>> 3bacb7a (adicionado as dlls para windows)
=======
REM Compila Cmake
cmake .. -G "MinGW Makefiles"

>>>>>>> b0749a674a6d46fbe05ab840a991c95440bb9010
REM Compila
mingw32-make

REM Executa o jogo
game.exe

pause