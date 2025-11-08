:: put gcc in PATH
@echo off
set W64DEVKIT_X64_BIN_PATH=C:\progs\w64devkit-x64\bin
set PATH=%W64DEVKIT_X64_BIN_PATH%;%PATH%

@echo on
:: project setup
set PROJECT_ROOT=C:\dev\raylib-base
set BUILD_OUT_PATH=%PROJECT_ROOT%\build\windows-debug-64
if not exist %BUILD_OUT_PATH% mkdir %BUILD_OUT_PATH%

gcc %PROJECT_ROOT%\main.c ^
    -Wall ^
    -Wextra ^
    -Werror ^
    -Wpedantic ^
    -Wshadow ^
    -Wconversion ^
    -Wsign-conversion ^
    -Wnull-dereference ^
    -g ^
    -O0 ^
-o %BUILD_OUT_PATH%\game.exe ^
-I %PROJECT_ROOT%\raylib-5.5\src ^
-L %PROJECT_ROOT%\raylib-5.5\release\windows-64 -lraylib -lgdi32 -lwinmm

if %ERRORLEVEL% neq 0 (
    echo Build failed.
    exit /b %ERRORLEVEL%
) else (
    echo Build succeeded.
    exit 0
)