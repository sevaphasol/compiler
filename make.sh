#!/bin/bash

# Переходим в директорию commons и выполняем сборку
cd commons || exit 1
make clean
make || exit 1

# Переходим в back-end и выполняем сборку
cd ../back-end || exit 1
make clean
make || exit 1

# Возвращаемся в корень и запускаем backend
cd .. || exit 1
back-end/build/backend frontend.txt backend.asm
