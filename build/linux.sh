#!/bin/bash

# Apaga cache antigo do CMake
rm -f CMakeCache.txt           # apaga o arquivo de cache
rm -rf CMakeFiles              # apaga a pasta de arquivos temporários do CMakez

#compilar
cmake .. -G "Unix Makefiles"

# Compila
make

# Executa o jogo
./game
