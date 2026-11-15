#!/bin/bash

gcc -std=c11 src/*.c -Iinclude -o graph

if [ $? -eq 0 ]; then
    ./graph
    rm graph
else
    echo "Erro na compilação"
    exit 1
fi
