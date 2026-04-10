@echo off
set GCC="C:\msys64\ucrt64\bin\gcc.exe"

echo Compiling...
%GCC% Main.c Admin.c Employee.c file.c -o program.exe

if %errorlevel% neq 0 (
    echo.
    echo Compilation FAILED. See errors above.
    pause
    exit /b 1
)

echo Compilation successful!
echo.
mkdir data 2>nul
program.exe
pause