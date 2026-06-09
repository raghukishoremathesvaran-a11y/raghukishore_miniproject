@echo off
set TCC_PATH="C:\Users\raghu\.gemini\antigravity-ide\scratch\compiler\tcc\tcc\tcc.exe"

echo Compiling main.exe...
%TCC_PATH% -O2 -o main.exe main.c graphics.c
if %errorlevel% neq 0 (
    echo Compilation of main.exe failed!
    exit /b %errorlevel%
)

echo Compiling test_graphics.exe...
%TCC_PATH% -O2 -o test_graphics.exe test_graphics.c graphics.c
if %errorlevel% neq 0 (
    echo Compilation of test_graphics.exe failed!
    exit /b %errorlevel%
)

echo Build successful!
