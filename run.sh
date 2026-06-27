#!/bin/bash

g++ src/main.cc -o main
./src/main | python visualizer/visualizer.py
