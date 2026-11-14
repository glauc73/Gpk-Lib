#!/bin/bash

gcc -std=c11 Gpk.c -o graph

if [ $? -eq 0 ]; then
    ./graph
else
    echo "Erro na compilação"
    exit 1
fi
