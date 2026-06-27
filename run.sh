#!/bin/bash

g++ src/main.cc -o main
./src/main $2 | python visualizer/visualizer.py
