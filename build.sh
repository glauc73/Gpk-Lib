#!/bin/bash

gcc -O3 -march=native -std=c11 src/*.c -Iinclude -o benchmark

if [ $? -eq 0 ]; then
    ./graph
    rm graph
else
    echo "Erro na compilação"
    exit 1
fi
