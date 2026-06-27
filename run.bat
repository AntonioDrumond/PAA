@echo off

g++ src/main.cc -o src/main.exe
.\src\main.exe %1 | python visualizer\visualizer.py
