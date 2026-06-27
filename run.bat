@echo off

g++ src/main.cc -o src/main.exe
.\src\main.exe | python visualizer\visualizer.py
