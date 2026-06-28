@echo off

g++ src/main.cc -o src/main.exe

set UNDIRECTED=0
set RANDOM=0

:: Parse up to 2 arguments
for %%A in ("%~1" "%~2") do (
    if /I "%%~A"=="undirected" set UNDIRECTED=1
    if /I "%%~A"=="random" set RANDOM=1
)

:: Run executable
if %RANDOM%==1 (
    .\src\main.exe random > out.txt
) else (
    .\src\main.exe > out.txt
)

:: Run visualizer
if %UNDIRECTED%==1 (
    python visualizer\visualizer.py --undirected out.txt
) else (
    python visualizer\visualizer.py out.txt
)
