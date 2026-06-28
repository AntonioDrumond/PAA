#!/bin/bash

g++ src/main.cc -o main

RAND="";
UNDIRECTED="";

if [ $# -eq 2 ]; then
    if [ "$1" == "random" ]; then
        RAND="random"
    elif [ "$1" == "undirected" ]; then
        UNDIRECTED="--undirected"
    fi
elif [ $# -eq 3 ]; then
    if [ "$1" == "random" ] ||[ "$2" == "random" ];  then
        RAND="random"
    elif [ "$1" == "undirected" ] ||[ "$2" == "undirected" ];  then
        UNDIRECTED="--undirected"
    fi
fi

./src/main $RAND > out.txt 
python visualizer/visualizer.py $UNDIRECTED out.txt
